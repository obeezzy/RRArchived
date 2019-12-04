#include "signupuser.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "config/config.h"
#include "database/databaseerror.h"
#include "database/queryrequest.h"
#include "database/databaseutils.h"
#include "database/databaseexception.h"

using namespace UserQuery;

SignUpUser::SignUpUser(const QString &userName, const QString &password, QObject *receiver) :
    UserExecutor(COMMAND, {
                    { "user_name", userName },
                    { "password", password }
                }, receiver)
{

}

QueryResult SignUpUser::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    QSqlDatabase connection;
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
    connection.setUserName(Config::instance().userName());
    connection.setPassword(Config::instance().password());
    connection.setConnectOptions("MYSQL_OPT_RECONNECT = 1");

    if (!connection.open())
        throw DatabaseException(DatabaseError::QueryErrorCode::SignInFailure,
                                connection.lastError().text(),
                                QString("Failed to open connection."));

    QSqlQuery q(connection);

    try {
        DatabaseUtils::beginTransaction(q);

        q.prepare("FLUSH PRIVILEGES");
        if (!q.exec())
            throw DatabaseException(DatabaseError::QueryErrorCode::SignUpFailure,
                                    q.lastError().text(),
                                    QString("Failed to flush privileges for '%1'.").arg(userName));

        q.prepare("CREATE USER :username @'localhost' IDENTIFIED BY :password");
        q.bindValue(":username", userName);
        q.bindValue(":password", password);
        if (!q.exec())
            throw DatabaseException(DatabaseError::QueryErrorCode::SignUpFailure,
                                    q.lastError().text(),
                                    QString("Failed to create user '%1'.").arg(userName));

        UserExecutor::grantPrivilege("SELECT", userName, q);
        UserExecutor::grantPrivilege("INSERT", userName, q);
        UserExecutor::grantPrivilege("UPDATE", userName, q);
        UserExecutor::grantPrivilege("DELETE", userName, q);

        q.prepare("FLUSH PRIVILEGES");
        if (!q.exec())
            throw DatabaseException(DatabaseError::QueryErrorCode::SignUpFailure,
                                    q.lastError().text(),
                                    QString("Failed to flush privileges for '%1'.").arg(userName));

        UserExecutor::createRRUser(userName, q);
        DatabaseUtils::commitTransaction(q);
        return result;
    } catch (DatabaseException &) {
        DatabaseUtils::rollbackTransaction(q);
        throw;
    }
}
