#include "usersqlmanager.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

#include "rr-core/database/queryrequest.h"
#include "rr-core/database/databaseexception.h"

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
    const QString &userName = request.params()["user_name"].toString();
    const QString &password = request.params()["password"].toString();

    if (connection().isOpen())
        connection().close();

    if (!QSqlDatabase::contains())
        connection() = QSqlDatabase::addDatabase("QMYSQL");
    else
        connection() = QSqlDatabase::database();

    connection().setDatabaseName("rr_temp");
    connection().setHostName("localhost");
    connection().setPort(3306);
    connection().setUserName(userName);
    connection().setPassword(password);
    connection().setConnectOptions("MYSQL_OPT_RECONNECT = 1");

    if (!connection().open() || !storeProfile(result))
        throw DatabaseException(DatabaseException::SignInFailure, connection().lastError().text(), QString("Failed to sign in as '%1'.").arg(userName));
}

void UserSqlManager::signUpUser(const QueryRequest &)
{

}
