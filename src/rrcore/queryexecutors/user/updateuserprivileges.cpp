#include "updateuserprivileges.h"

using namespace UserQuery;

UpdateUserPrivileges::UpdateUserPrivileges(int userId,
                                           QObject *receiver) :
    UserExecutor(COMMAND, {
                    { "user_id", userId }
                 }, receiver)
{

}

QueryResult UpdateUserPrivileges::execute()
{
    QueryResult result;

    return result;
}
