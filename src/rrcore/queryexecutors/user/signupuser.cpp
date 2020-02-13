#include "signupuser.h"
#include "database/exceptions/exceptions.h"
#include "utility/userutils.h"
#include "config/config.h"
#include "database/databaseerror.h"
#include "database/queryrequest.h"
#include "database/databaseutils.h"
#include "database/databaseexception.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

using namespace UserQuery;

SignUpUser::SignUpUser(const Utility::User &user,
                       QObject *receiver) :
    UserExecutor(COMMAND, {
                    { "user", user.user },
                    { "password", user.password }
                }, receiver)
{

}

QueryResult SignUpUser::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    const QVariantMap &params = request().params();
    const QString &userName = params.value("user").toString();

    QSqlDatabase connection;
    connectToDatabase(connection);

    QSqlQuery q(connection);

    try {
        DatabaseUtils::beginTransaction(q);

        addSqlUser(q);
        UserExecutor::createRRUser(userName, q);

        DatabaseUtils::commitTransaction(q);
        return result;
    } catch (DatabaseException &) {
        DatabaseUtils::rollbackTransaction(q);
        throw;
    }
}

void SignUpUser::connectToDatabase(QSqlDatabase &connection)
{
    const QString &userName = request().params().value("user_name").toString();
    const QString &password = request().params().value("password").toString();

    if (connection.isOpen())
        connection.close();

    if (!QSqlDatabase::contains())
        connection = QSqlDatabase::addDatabase("QMYSQL", connectionName());
    else
        connection = QSqlDatabase::database(connectionName());

    connection.setDatabaseName(Config::instance().databaseName());
    connection.setHostName(Config::instance().hostName());
    connection.setPort(Config::instance().port());
    connection.setUserName(userName);
    connection.setPassword(password);
    connection.setConnectOptions("MYSQL_OPT_RECONNECT = 1");

    if (!connection.open())
        throw DatabaseException(DatabaseError::QueryErrorCode::SignInFailure,
                                connection.lastError().text(),
                                QString("Failed to open connection."));
}

void SignUpUser::addSqlUser(QSqlQuery &q)
{
    const QString &userName = request().params().value("user_name").toString();
    const QString &password = request().params().value("password").toString();

    q.prepare("FLUSH PRIVILEGES");
    if (!q.exec())
        throw SqlStatementFailedException(QStringLiteral("Failed to flush privileges for '%1'.")
                                          .arg(userName),
                                          q.lastError().text());

    q.prepare("CREATE USER :username @'localhost' IDENTIFIED BY :password");
    q.bindValue(":username", userName);
    q.bindValue(":password", password);
    if (!q.exec())
        throw SqlStatementFailedException(QStringLiteral("Failed to create user '%1'.")
                                          .arg(userName),
                                          q.lastError().text());


    UserExecutor::grantPrivilege("SELECT", userName, q);
    UserExecutor::grantPrivilege("INSERT", userName, q);
    UserExecutor::grantPrivilege("UPDATE", userName, q);
    UserExecutor::grantPrivilege("DELETE", userName, q);

    q.prepare("FLUSH PRIVILEGES");
    if (!q.exec())
        throw SqlStatementFailedException(QStringLiteral("Failed to flush privileges for '%1'.")
                                          .arg(userName),
                                          q.lastError().text());
}
