#include "viewuserdetails.h"

using namespace UserQuery;

ViewUserDetails::ViewUserDetails(int userId,
                                 QObject *receiver) :
    UserExecutor(COMMAND, {
                    { "user_id", userId }
                 }, receiver)
{

}

QueryResult ViewUserDetails::execute()
{
    QueryResult result { request() };

    return result;
}
