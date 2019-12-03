#include "viewuserprivileges.h"

using namespace UserQuery;

ViewUserPrivileges::ViewUserPrivileges(int userId,
                                       QObject *receiver) :
    UserExecutor(COMMAND, {
                    { "user_id", userId }
                 }, receiver)
{

}

QueryResult ViewUserPrivileges::execute()
{
    QueryResult result;

    return result;
}
