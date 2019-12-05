#include "databasecreator.h"
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFile>
#include <QFileInfo>
#include <QSqlError>
#include <QSqlDriver>
#include <QRegularExpression>
#include <QDebug>
#include <QDateTime>
#include <QDirIterator>
#include <QUrl>
#include <QSettings>
#include <QByteArray>

#include "database/databaseexception.h"
#include "database/databaseutils.h"
#include "config/config.h"
#include "schema/schema.h"
#include "user/userprofile.h"
#include "user/businessdetails.h"

Q_LOGGING_CATEGORY(databaseCreator, "rrcore.database.databasecreator");

const QString CONNECTION_NAME("databasecreator");

const QString PROCEDURE_SEPARATOR("---");
const QString SPACES_AND_TABS_PATTERN("(\\/\\*(.|\\n)*?\\*\\/|^--.*\\n|\\t|\\n)"); // Replace comments and tabs and new lines with space
const QString DATABASE_NAME_PATTERN("###DATABASENAME###");

DatabaseCreator::DatabaseCreator(QSqlDatabase connection) :
    m_connection(connection)
{
    if (!m_connection.isValid()) {
        if (!QSqlDatabase::contains(CONNECTION_NAME))
            m_connection = QSqlDatabase::addDatabase("QMYSQL", CONNECTION_NAME);
        else
            m_connection = QSqlDatabase::database(CONNECTION_NAME);

        // Disconnect and connect to 'mysql'
        if (m_connection.isOpen())
            m_connection.close();

        m_connection.setDatabaseName("mysql");
        m_connection.setHostName(Config::instance().hostName());
        m_connection.setPort(Config::instance().port());
        m_connection.setUserName(Config::instance().userName());
        m_connection.setPassword(Config::instance().password());
        m_connection.setConnectOptions("MYSQL_OPT_RECONNECT = 1");

        m_connection.open();
    }
}

void DatabaseCreator::executeSqlFile(const QString &fileName)
{
    if (fileName.trimmed().isEmpty())
        return;

    QFile file(fileName);
    if (QFileInfo(fileName).suffix() != "sql")
        throw DatabaseException(DatabaseError::QueryErrorCode::DatabaseInitializationFailed, QString(),
                                QString("File '%1' is not a sql file").arg(fileName));
    if (QFileInfo(fileName).size() > 1024 * 50)
        throw DatabaseException(DatabaseError::QueryErrorCode::DatabaseInitializationFailed, QString(),
                                QString("File '%1' is too large (larger than 50MB).").arg(fileName));
    if (!file.open(QFile::ReadOnly))
        throw DatabaseException(DatabaseError::QueryErrorCode::DatabaseInitializationFailed, QString(),
                                QString("Failed to open '%1'").arg(fileName));

    QSqlQuery q(m_connection);
    QString sqlData = file.readAll();

    if (Config::instance().databaseName().toLower() == QStringLiteral("mysql"))
        throw DatabaseException(DatabaseError::QueryErrorCode::DatabaseInitializationFailed,
                                q.lastError().text(), "Database name cannot be mysql.");

    // Inject database name
    sqlData = sqlData.replace(QRegularExpression(DATABASE_NAME_PATTERN), Config::instance().databaseName());

    if(m_connection.driver()->hasFeature(QSqlDriver::Transactions)) {
        // Replace comments and tabs and new lines with space
        sqlData = sqlData.replace(QRegularExpression("(\\/\\*(.|\\n)*?\\*\\/|^--.*\\n|\\t|\\n)",
                                                     QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption), " ");
        // Remove waste spaces
        sqlData = sqlData.trimmed();

        // Extract queries
        QStringList extractedQueries = sqlData.split(';', QString::SkipEmptyParts);

        // Initialize regular expression for detecting special queries (`begin transaction` and `commit`).
        QRegularExpression re_transaction("\\bbegin.transaction.*", QRegularExpression::CaseInsensitiveOption);
        QRegularExpression re_commit("\\bcommit.*", QRegularExpression::CaseInsensitiveOption);

        // Check if query file is already wrapped with a transaction
        bool isStartedWithTransaction = re_transaction.match(extractedQueries.at(0)).hasMatch();
        if(!isStartedWithTransaction)
            m_connection.transaction();     //<=== not wrapped with a transaction, so we wrap it with a transaction.

        //Execute each individual queries
        for(const QString &s : extractedQueries) {
            if(re_transaction.match(s).hasMatch())    //<== detecting special query
                m_connection.transaction();
            else if(re_commit.match(s).hasMatch())    //<== detecting special query
                m_connection.commit();
            else {
                q.exec(s);                        //<== execute normal query
                if(q.lastError().type() != QSqlError::NoError) {
                    qCInfo(databaseCreator) << q.lastError().text();
                    m_connection.rollback();                    //<== rollback the transaction if there is any problem
                }
            }
        }
        if(!isStartedWithTransaction)
            m_connection.commit();          //<== ... completing of wrapping with transaction

        //Sql Driver doesn't supports transaction
    } else {
        //...so we need to remove special queries (`begin transaction` and `commit`)
        sqlData = sqlData.replace(QRegularExpression("(\\bbegin.transaction.*;|\\bcommit.*;|\\/\\*(.|\\n)*?\\*\\/|^--.*\\n|\\t|\\n)",
                                                     QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption), " ");
        sqlData = sqlData.trimmed();

        //Execute each individual queries
        QStringList extractedQueries = sqlData.split(';', QString::SkipEmptyParts);
        for (const QString &s : extractedQueries) {
            q.exec(s);
            if(q.lastError().type() != QSqlError::NoError)
                qCInfo(databaseCreator) << q.lastError().text();
        }
    }
}

bool DatabaseCreator::start()
{
    try {
        QSettings settings;

        // NOTE: Only drop if database was not ready
        if (!UserProfile::instance().isDatabaseReady())
            dropDatabase();
        initDatabase();
        createProcedures();
        updateBusinessDetails();
    } catch (DatabaseException &e) {
        qCCritical(databaseCreator) << "Exception caught:" << e.code() << e.message() << e.userMessage();
        return false;
    }

    return true;
}

void DatabaseCreator::dropDatabase()
{
    QSqlQuery q(m_connection);
    q.prepare(QStringLiteral("DROP DATABASE IF EXISTS %1").arg(Config::instance().databaseName()));

    if (!q.exec())
        throw DatabaseException(DatabaseError::QueryErrorCode::DatabaseInitializationFailed,
                                q.lastError().text(),
                                QStringLiteral("Failed to drop database!"));
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
        if (QFileInfo(file).suffix() != "sql")
            continue;
        if (!file.open(QFile::ReadOnly))
            throw DatabaseException(DatabaseError::QueryErrorCode::DatabaseInitializationFailed,
                                    file.errorString(),
                                    QStringLiteral("Failed to create procedures!"));

        executeStoredProcedures(QFileInfo(file).filePath());
    }
}

void DatabaseCreator::updateBusinessDetails()
{
    QSqlQuery q(m_connection);
    q.prepare(QStringLiteral("CALL UpdateBusinessDetails('%1', '%2', '%3', %4, '%5', '%6', NULL)")
              .arg(UserProfile::instance().businessDetails()->name())
              .arg(UserProfile::instance().businessDetails()->address())
              .arg(UserProfile::instance().businessDetails()->businessFamily())
              .arg(UserProfile::instance().businessDetails()->establishmentYear())
              .arg(UserProfile::instance().businessDetails()->phoneNumber())
              .arg(QString(DatabaseUtils::imageUrlToByteArray(UserProfile::instance().businessDetails()->logoUrl()))));

    if (!q.exec())
        throw DatabaseException(DatabaseError::QueryErrorCode::DatabaseInitializationFailed,
                                q.lastError().text(),
                                QStringLiteral("Failed to update business details table!"));
}

void DatabaseCreator::executeStoredProcedures(const QString &fileName)
{
    if (fileName.trimmed().isEmpty())
        return;

    QFile file(fileName);
    if (QFileInfo(fileName).suffix() != "sql")
        throw DatabaseException(DatabaseError::QueryErrorCode::DatabaseInitializationFailed, QString(),
                                QStringLiteral("File '%1' is not a sql file").arg(fileName));
    if (QFileInfo(fileName).size() > 1024 * 50)
        throw DatabaseException(DatabaseError::QueryErrorCode::DatabaseInitializationFailed, QString(),
                                QStringLiteral("File '%1' is too large (larger than 50MB).").arg(fileName));
    if (!file.open(QFile::ReadOnly))
        throw DatabaseException(DatabaseError::QueryErrorCode::DatabaseInitializationFailed, QString(),
                                QStringLiteral("Failed to open '%1'").arg(fileName));

    QSqlQuery q(m_connection);
    file.seek(0);
    QString sqlData = file.readAll();

    if (Config::instance().databaseName().toLower() == QStringLiteral("mysql"))
        throw DatabaseException(DatabaseError::QueryErrorCode::DatabaseInitializationFailed,
                                q.lastError().text(), "Database name cannot be mysql.");

    // Inject database name
    sqlData = sqlData.replace(QRegularExpression(DATABASE_NAME_PATTERN), Config::instance().databaseName());

    QStringList statements = sqlData.split(PROCEDURE_SEPARATOR);
    for (auto &statement : statements) {
        // Remove waste spaces
        statement = statement.trimmed();

        // Replace comments and tabs and new lines with space
        statement = statement.replace(QRegularExpression(SPACES_AND_TABS_PATTERN,
                                                     QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption), " ");
        // Remove waste spaces
        statement = statement.trimmed();

        if (!q.exec(statement)) {
            qCCritical(databaseCreator) << "Invalid statement=====" << statement;
            throw DatabaseException(DatabaseError::QueryErrorCode::DatabaseInitializationFailed,
                                    QString("Failed to execute query: %1").arg(statement));
        }
    }
}
