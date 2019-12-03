#include "activateuser.h"

using namespace UserQuery;

ActivateUser::ActivateUser(const QString &userName,
                           bool active,
                           QObject *receiver) :
    UserExecutor(COMMAND, {
                    { "user_name", userName },
                    { "active", active }
                 }, receiver)
{

}

QueryResult ActivateUser::execute()
{
    QueryResult result;

    return result;
}
