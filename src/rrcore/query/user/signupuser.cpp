#include "signupuser.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "config/config.h"
#include "database/databaseerror.h"
#include "database/databaseexception.h"
#include "database/exceptions/exceptions.h"
#include "database/queryrequest.h"
#include "utility/user/user.h"

using namespace Query::User;

SignUpUser::SignUpUser(const Utility::User::User& user, QObject* receiver)
    : UserExecutor(COMMAND, {{"user", user.user}, {"password", user.password}},
                   receiver)
{}

QueryResult SignUpUser::execute()
{
    QueryResult result{request()};
    result.setSuccessful(true);

    const QVariantMap& params{request().params()};
    const QString& userName = params.value("user").toString();

    QSqlDatabase connection;
    connectToDatabase(connection);

    QSqlQuery q(connection);

    try {
        QueryExecutor::beginTransaction(q);

        addSqlUser(q);
        UserExecutor::createRRUser(userName, q);

        QueryExecutor::commitTransaction(q);
        return result;
    } catch (const DatabaseException&) {
        QueryExecutor::rollbackTransaction(q);
        throw;
    }
}

void SignUpUser::connectToDatabase(QSqlDatabase& connection)
{
    const QString& userName = request().params().value("user_name").toString();
    const QString& password = request().params().value("password").toString();

    if (connection.isOpen())
        connection.close();

    if (!QSqlDatabase::contains())
        connection = QSqlDatabase::addDatabase("QPSQL", connectionName());
    else
        connection = QSqlDatabase::database(connectionName());

    connection.setDatabaseName(Config::instance().databaseName());
    connection.setHostName(Config::instance().hostName());
    connection.setPort(Config::instance().port());
    connection.setUserName(userName);
    connection.setPassword(password);
    connection.setConnectOptions();

    if (!connection.open())
        throw ConnectionFailedException(
            QStringLiteral("Failed to open connection."),
            connection.lastError());
}

void SignUpUser::addSqlUser(QSqlQuery& q)
{
    const QString& userName = request().params().value("user_name").toString();
    const QString& password = request().params().value("password").toString();

    q.prepare("FLUSH PRIVILEGES");
    if (!q.exec())
        throw SqlStatementFailedException(
            QStringLiteral("Failed to flush privileges for '%1'.")
                .arg(userName),
            q.lastError().text());

    q.prepare("CREATE USER :username @'localhost' IDENTIFIED BY :password");
    q.bindValue(":username", userName);
    q.bindValue(":password", password);
    if (!q.exec())
        throw SqlStatementFailedException(
            QStringLiteral("Failed to create user '%1'.").arg(userName),
            q.lastError().text());

    UserExecutor::grantPrivilege("SELECT", userName, q);
    UserExecutor::grantPrivilege("INSERT", userName, q);
    UserExecutor::grantPrivilege("UPDATE", userName, q);
    UserExecutor::grantPrivilege("DELETE", userName, q);

    q.prepare("FLUSH PRIVILEGES");
    if (!q.exec())
        throw SqlStatementFailedException(
            QStringLiteral("Failed to flush privileges for '%1'.")
                .arg(userName),
            q.lastError().text());
}
