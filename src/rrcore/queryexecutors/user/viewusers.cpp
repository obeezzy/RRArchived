#include "viewusers.h"

using namespace UserQuery;

ViewUsers::ViewUsers(bool archived,
                     QObject *receiver) :
    UserExecutor(COMMAND, {
                    { "archived", archived }
                 }, receiver)
{

}

QueryResult ViewUsers::execute()
{
    QueryResult result;

    return result;
}
