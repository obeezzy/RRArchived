#include "removeuser.h"

using namespace UserQuery;

RemoveUser::RemoveUser(const QString &userName,
                       QObject *receiver) :
    UserExecutor(COMMAND, {
                        { "user_name", userName }
                 }, receiver)
{

}

QueryResult RemoveUser::execute()
{
    QueryResult result{ request() };

    return result;
}
