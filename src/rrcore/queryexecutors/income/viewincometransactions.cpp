#include "viewincometransactions.h"

using namespace IncomeQuery;

ViewIncomeTransactions::ViewIncomeTransactions(bool archived,
                                               QObject *receiver) :
    IncomeExecutor(COMMAND, {
                        { "archived", archived }
                   }, receiver)
{

}

QueryResult ViewIncomeTransactions::execute()
{
    QueryResult result;

    return result;
}
