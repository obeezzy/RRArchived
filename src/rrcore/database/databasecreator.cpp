#include "databasecreator.h"
#include <QByteArray>
#include <QDateTime>
#include <QDebug>
#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include <QRegularExpression>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QUrl>
#include "config/config.h"
#include "database/databaseexception.h"
#include "database/exceptions/exceptions.h"
#include "schema/schema.h"
#include "user/businessdetails.h"
#include "user/userprofile.h"

Q_LOGGING_CATEGORY(lcdatabasecreator, "rrcore.database.databasecreator");

const int MAX_FILE_SIZE{1024 * 50};
const QString CONNECTION_NAME("databasecreator");

const QString PROCEDURE_SEPARATOR("---");
const QString SPACES_AND_TABS_PATTERN(
    "(\\/\\*(.|\\n)*?\\*\\/|^--.*\\n|\\t|\\n)");  // Replace comments and tabs
                                                  // and new lines with space

DatabaseCreator::DatabaseCreator(QSqlDatabase connection)
    : m_connection(connection)
{}

void DatabaseCreator::openConnection(const QString& databaseName)
{
    if (!m_connection.isValid()) {
        if (!QSqlDatabase::contains(CONNECTION_NAME))
            m_connection = QSqlDatabase::addDatabase("QPSQL", CONNECTION_NAME);
        else
            m_connection = QSqlDatabase::database(CONNECTION_NAME);

        if (m_connection.isOpen())
            m_connection.close();

        m_connection.setDatabaseName(databaseName);
        m_connection.setHostName(Config::instance().hostName());
        m_connection.setPort(Config::instance().port());
        m_connection.setUserName(Config::instance().userName());
        m_connection.setPassword(Config::instance().password());
        m_connection.setConnectOptions();

        m_connection.open();
    }
}

void DatabaseCreator::executeSqlFile(const QString& fileName)
{
    if (fileName.trimmed().isEmpty())
        return;

    QFile file(fileName);
    if (QFileInfo(fileName).suffix() != QStringLiteral("sql"))
        throw DatabaseInitializationFailedException(
            QStringLiteral("File '%1' is not a sql file").arg(fileName));
    if (QFileInfo(fileName).size() > 1024 * 50)
        throw DatabaseInitializationFailedException(
            QStringLiteral("File '%1' is too large (larger than 50MB).")
                .arg(fileName));
    if (!file.open(QFile::ReadOnly))
        throw DatabaseInitializationFailedException(
            QStringLiteral("Failed to open '%1'").arg(fileName));

    QSqlQuery q(m_connection);
    QString sqlData = file.readAll();

    if (Config::instance().databaseName().toLower() ==
        QStringLiteral("postgres"))
        throw DatabaseInitializationFailedException(
            QStringLiteral("Database name cannot be postgres."));

    if (m_connection.driver()->hasFeature(QSqlDriver::Transactions)) {
        // Replace comments and tabs and new lines with space
        sqlData = sqlData.replace(
            QRegularExpression(SPACES_AND_TABS_PATTERN,
                               QRegularExpression::CaseInsensitiveOption |
                                   QRegularExpression::MultilineOption),
            QStringLiteral(" "));
        // Remove waste spaces
        sqlData = sqlData.trimmed();

        // Extract queries
        QStringList extractedQueries =
            sqlData.split(';', QString::SkipEmptyParts);

        // Initialize regular expression for detecting special queries (`begin
        // transaction` and `commit`).
        QRegularExpression re_transaction(
            "\\bbegin.transaction.*",
            QRegularExpression::CaseInsensitiveOption);
        QRegularExpression re_commit("\\bcommit.*",
                                     QRegularExpression::CaseInsensitiveOption);

        // Check if query file is already wrapped with a transaction
        bool isStartedWithTransaction =
            re_transaction.match(extractedQueries.at(0)).hasMatch();
        if (!isStartedWithTransaction)
            m_connection
                .transaction();  //<=== not wrapped with a transaction, so we
                                 // wrap it with a transaction.

        // Execute each individual queries
        for (const QString& s : extractedQueries) {
            if (re_transaction.match(s)
                    .hasMatch())  //<== detecting special query
                m_connection.transaction();
            else if (re_commit.match(s)
                         .hasMatch())  //<== detecting special query
                m_connection.commit();
            else {
                q.exec(s);  //<== execute normal query
                if (q.lastError().type() != QSqlError::NoError) {
                    qCInfo(lcdatabasecreator) << q.lastError().text();
                    m_connection
                        .rollback();  //<== rollback the transaction if there is
                                      // any problem
                }
            }
        }
        if (!isStartedWithTransaction)
            m_connection
                .commit();  //<== ... completing of wrapping with transaction

        // Sql Driver doesn't supports transaction
    } else {
        sqlData = sqlData.trimmed();
        // Execute each individual queries
        QStringList extractedQueries =
            sqlData.split(';', QString::SkipEmptyParts);
        for (const QString& s : extractedQueries) {
            q.exec(s);
            if (q.lastError().type() != QSqlError::NoError)
                qCInfo(lcdatabasecreator) << q.lastError().text();
        }
    }
}

bool DatabaseCreator::start()
{
    try {
        QSettings settings;

        openConnection("postgres");
        if (!UserProfile::instance().isDatabaseReady())
            dropDatabase();
        createDatabase();
        openConnection(Config::instance().databaseName());
        initDatabase();
        createProcedures();
        updateBusinessDetails();
    } catch (const DatabaseException& e) {
        qCCritical(lcdatabasecreator) << e;
        return false;
    }

    return true;
}

void DatabaseCreator::createDatabase()
{
    QSqlQuery q(m_connection);
    q.prepare(QStringLiteral("CREATE DATABASE %1")
                  .arg(Config::instance().databaseName()));

    if (!q.exec())
        throw DatabaseInitializationFailedException(
            QStringLiteral("Failed to create database!"), q.lastError());
}

void DatabaseCreator::dropDatabase()
{
    QSqlQuery q(m_connection);
    q.prepare(QStringLiteral("DROP DATABASE IF EXISTS %1")
                  .arg(Config::instance().databaseName()));

    if (!q.exec())
        throw DatabaseInitializationFailedException(
            QStringLiteral("Failed to drop database!"), q.lastError());
}

void DatabaseCreator::initDatabase()
{
    executeSqlFile(Schema::Common::INIT_SQL_FILE);
}

void DatabaseCreator::createProcedures()
{
    QDirIterator iter(Schema::Common::PROCEDURE_DIR);
    while (iter.hasNext()) {
        QFile file(iter.next());
        if (QFileInfo(file).suffix() != QStringLiteral("sql"))
            continue;
        if (!file.open(QFile::ReadOnly))
            throw DatabaseInitializationFailedException(
                QStringLiteral("Failed to create procedures!"));

        executeStoredProcedures(QFileInfo(file).filePath());
    }
}

void DatabaseCreator::updateBusinessDetails()
{
    QSqlQuery q(m_connection);
    q.prepare(
        QStringLiteral(
            "SELECT UpdateBusinessDetails('%1', '%2', '%3', %4, '%5', "
            "'%6', NULL)")
            .arg(UserProfile::instance().businessDetails()->name())
            .arg(UserProfile::instance().businessDetails()->address())
            .arg(UserProfile::instance().businessDetails()->businessFamily())
            .arg(UserProfile::instance().businessDetails()->establishmentYear())
            .arg(UserProfile::instance().businessDetails()->phoneNumber())
            .arg(QString(UserProfile::instance().businessDetails()->logo())));

    if (!q.exec())
        throw DatabaseInitializationFailedException(
            QStringLiteral("Failed to update business details table!"),
            q.lastError());
}

void DatabaseCreator::executeStoredProcedures(const QString& fileName)
{
    if (fileName.trimmed().isEmpty())
        return;

    QFile file(fileName);
    if (QFileInfo(fileName).suffix() != QStringLiteral("sql"))
        throw DatabaseInitializationFailedException(
            QStringLiteral("File '%1' is not a sql file").arg(fileName));
    if (QFileInfo(fileName).size() > MAX_FILE_SIZE)
        throw DatabaseInitializationFailedException(
            QStringLiteral("File '%1' is too large (larger than 50MB).")
                .arg(fileName));
    if (!file.open(QFile::ReadOnly))
        throw DatabaseInitializationFailedException(
            QStringLiteral("Failed to open '%1'").arg(fileName));

    QSqlQuery q(m_connection);
    file.seek(0);
    QString sqlData = file.readAll();

    if (Config::instance().databaseName().toLower() ==
        QStringLiteral("postgres"))
        throw DatabaseInitializationFailedException(
            QStringLiteral("Database name cannot be postgres."));

    QStringList statements = sqlData.split(PROCEDURE_SEPARATOR);
    for (auto& statement : statements) {
        // Remove waste spaces
        statement = statement.trimmed();

        // Replace comments and tabs and new lines with space
        statement = statement.replace(
            QRegularExpression(SPACES_AND_TABS_PATTERN,
                               QRegularExpression::CaseInsensitiveOption |
                                   QRegularExpression::MultilineOption),
            " ");
        // Remove waste spaces
        statement = statement.trimmed();

        if (!q.exec(statement))
            throw SqlStatementFailedException(statement, q.lastError());
    }
}
