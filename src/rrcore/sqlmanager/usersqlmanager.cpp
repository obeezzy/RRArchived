#include "usersqlmanager.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QSettings>
#include <QDateTime>

#include "database/queryrequest.h"
#include "database/databaseexception.h"
#include "database/databaseutils.h"
#include "config/config.h"

UserSqlManager::UserSqlManager(QSqlDatabase connection)
    : AbstractSqlManager(connection)
{

}

QueryResult UserSqlManager::execute(const QueryRequest &request)
{
    QueryResult result(request);

    try {
        if (request.command() == "sign_in_user")
            signInUser(request, result);
        else if (request.command() == "sign_up_user")
            signUpUser(request);
        else if (request.command() == "sign_up_root_user")
            signUpRootUser(request);
        else if (request.command() == "remove_user")
            removeUser(request);
        else
            throw DatabaseException(DatabaseException::RRErrorCode::CommandNotFound, QString("Command not found: %1").arg(request.command()));

        result.setSuccessful(true);
    } catch (DatabaseException &e) {
        result.setSuccessful(false);
        result.setErrorCode(e.code());
        result.setErrorMessage(e.message());
        result.setErrorUserMessage(e.userMessage());
    }

    return result;
}

bool UserSqlManager::storeProfile(QueryResult &result)
{
    if (connection().isOpen()) {
        QSqlQuery q(connection());
        q.prepare("SELECT id, user FROM user");
        if (!q.exec())
            return false;

        if (q.first())
            result.setOutcome(recordToMap(q.record()));

        return true;
    }

    return false;
}

void UserSqlManager::signInUser(const QueryRequest &request, QueryResult result)
{
    const QString &userName = request.params().value("user_name").toString();
    const QString &password = request.params().value("password").toString();

    if (connection().isOpen())
        connection().close();

    if (!QSqlDatabase::contains())
        connection() = QSqlDatabase::addDatabase("QMYSQL");
    else
        connection() = QSqlDatabase::database();

    connection().setDatabaseName(Config::instance().databaseName());
    connection().setHostName(Config::instance().hostName());
    connection().setPort(Config::instance().port());
    connection().setUserName(userName);
    connection().setPassword(password);
    connection().setConnectOptions("MYSQL_OPT_RECONNECT = 1");

    if (!connection().open() || !storeProfile(result))
        throw DatabaseException(DatabaseException::RRErrorCode::SignInFailure, connection().lastError().text(),
                                QString("Failed to sign in as '%1'.").arg(userName));
}

void UserSqlManager::signUpUser(const QueryRequest &request)
{
    const QString &userName = request.params().value("user_name").toString();
    const QString &password = request.params().value("password").toString();

    if (connection().isOpen())
        connection().close();

    if (!QSqlDatabase::contains())
        connection() = QSqlDatabase::addDatabase("QMYSQL");
    else
        connection() = QSqlDatabase::database();

    connection().setDatabaseName(Config::instance().databaseName());
    connection().setHostName(Config::instance().hostName());
    connection().setPort(Config::instance().port());
    connection().setUserName(Config::instance().userName());
    connection().setPassword(Config::instance().password());
    connection().setConnectOptions("MYSQL_OPT_RECONNECT = 1");

    if (!connection().open())
        throw DatabaseException(DatabaseException::RRErrorCode::SignInFailure, connection().lastError().text(),
                                QString("Failed to open connection."));

    QSqlQuery q(connection());

    try {
        if (!DatabaseUtils::beginTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::BeginTransactionFailed, q.lastError().text(), "Failed to start transation.");

        q.prepare("FLUSH PRIVILEGES");
        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::SignUpFailure, q.lastError().text(),
                                    QString("Failed to flush privileges for '%1'.").arg(userName));

        q.prepare("CREATE USER :username @'localhost' IDENTIFIED BY :password");
        q.bindValue(":username", userName);
        q.bindValue(":password", password);
        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::SignUpFailure, q.lastError().text(),
                                    QString("Failed to create user '%1'.").arg(userName));

        grantPrivilege("SELECT", userName, q);
        grantPrivilege("INSERT", userName, q);
        grantPrivilege("UPDATE", userName, q);
        grantPrivilege("DELETE", userName, q);

        q.prepare("FLUSH PRIVILEGES");
        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::SignUpFailure, q.lastError().text(),
                                    QString("Failed to flush privileges for '%1'.").arg(userName));

        createRRUser(userName, q);

        if (!DatabaseUtils::commitTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::CommitTransationFailed, q.lastError().text(), "Failed to commit transation.");

    } catch (DatabaseException &) {
        if (!DatabaseUtils::rollbackTransaction(q))
            qCritical("Failed to rollback failed transaction! %s", q.lastError().text().toStdString().c_str());

        throw;
    }
}

void UserSqlManager::signUpRootUser(const QueryRequest &request)
{
    const QString &userName = request.params().value("user_name").toString();
    const QString &password = request.params().value("password").toString();

    if (connection().isOpen())
        connection().close();

    connection().setDatabaseName("mysql");

    if (!QSqlDatabase::contains())
        connection() = QSqlDatabase::addDatabase("QMYSQL");
    else
        connection() = QSqlDatabase::database();

    connection().setDatabaseName(Config::instance().databaseName());
    connection().setHostName(Config::instance().hostName());
    connection().setPort(Config::instance().port());
    connection().setUserName(userName);
    connection().setPassword(password);
    connection().setConnectOptions("MYSQL_OPT_RECONNECT = 1");

    QSqlQuery q(connection());

    try {
        if (!DatabaseUtils::beginTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::BeginTransactionFailed, q.lastError().text(), "Failed to start transation.");

        q.prepare(QString("CREATE USER '%1'@'localhost' IDENTIFIED BY '%2'").arg(userName, password));
        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::SignUpFailure, q.lastError().text(),
                                    QString("Failed to create root user '%1'.").arg(userName));

        grantPrivilege("ALL PRIVILEGES", userName, q);

        q.prepare("FLUSH PRIVILEGES");
        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::SignUpFailure, q.lastError().text(),
                                    QString("Failed to flush privileges for '%1'.").arg(userName));

        createRRUser(userName, q);

        if (!DatabaseUtils::commitTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::CommitTransationFailed, q.lastError().text(), "Failed to commit transation.");

    } catch (DatabaseException &) {
        if (!DatabaseUtils::rollbackTransaction(q))
            qCritical("Failed to rollback failed transaction! %s", q.lastError().text().toStdString().c_str());

        throw;
    }
}

void UserSqlManager::removeUser(const QueryRequest &request)
{
    const QString &userName = request.params().value("user_name").toString();

    if (!QSqlDatabase::contains())
        connection() = QSqlDatabase::addDatabase("QMYSQL");
    else
        connection() = QSqlDatabase::database();

    if (!connection().isOpen())
        throw DatabaseException(DatabaseException::RRErrorCode::RemoveUserFailure,
                                connection().lastError().text(), QString("Connection is closed."));

    QSqlQuery q(connection());
    q.prepare("DROP USER :user_name@'localhost'");
    q.bindValue(":user_name", userName);

    if (!q.exec())
        throw DatabaseException(DatabaseException::RRErrorCode::RemoveUserFailure,
                                q.lastError().text(),
                                QString("Failed to drop user '%1'.").arg(userName));
}

void UserSqlManager::grantPrivilege(const QString &privilege, const QString &userName, QSqlQuery &q)
{
    q.prepare(QString("GRANT %1 ON * . * TO :username@'localhost'").arg(privilege));
    q.bindValue(":username", userName);
    if (!q.exec())
        throw DatabaseException(DatabaseException::RRErrorCode::SignUpFailure, q.lastError().text(),
                                QString("Failed to grant '%1' privileges to '%2'.")
                                .arg(privilege, userName));
}

void UserSqlManager::createRRUser(const QString &userName, QSqlQuery &q)
{
    const QDateTime currentDateTime = QDateTime::currentDateTime();

    q.prepare(QString("INSERT INTO %1.user (user, photo, phone_number, email_address, active, pending, created, last_edited) "
              "VALUES (:user, :photo, :phone_number, :email_address, :active, :pending, :created, :last_edited)")
              .arg(Config::instance().databaseName()));
    q.bindValue(":user", userName);
    q.bindValue(":photo", QVariant(QVariant::ByteArray));
    q.bindValue(":phone_number", QVariant(QVariant::String));
    q.bindValue(":email_address", QVariant(QVariant::String));
    q.bindValue(":active", false);
    q.bindValue(":pending", QVariant(QVariant::Bool));
    q.bindValue(":created", currentDateTime);
    q.bindValue(":last_edited", currentDateTime);

    if (!q.exec())
        throw DatabaseException(DatabaseException::RRErrorCode::SignUpFailure, q.lastError().text(),
                                QString("Failed to create RR user '%1'.").arg(userName));
}
