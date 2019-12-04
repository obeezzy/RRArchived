#include "signoutuser.h"
#include <QSqlDatabase>

using namespace UserQuery;

SignOutUser::SignOutUser(QObject *receiver) :
    UserExecutor(COMMAND, { }, receiver)
{

}

QueryResult SignOutUser::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    connection.close();

    return result;
}
