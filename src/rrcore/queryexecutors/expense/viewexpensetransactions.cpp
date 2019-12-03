#include "viewexpensetransactions.h"

using namespace ExpenseQuery;

ViewExpenseTransactions::ViewExpenseTransactions(bool archived,
                                                 QObject *receiver) :
    ExpenseExecutor(COMMAND, {
                        { "archived", archived }
                    }, receiver)
{

}

QueryResult ViewExpenseTransactions::execute()
{
    QueryResult result{ request() };

    return result;
}
