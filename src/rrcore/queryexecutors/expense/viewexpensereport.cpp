#include "viewexpensereport.h"

using namespace ExpenseQuery;

ViewExpenseReport::ViewExpenseReport(QObject *receiver) :
    ExpenseExecutor(COMMAND, {}, receiver)
{

}

QueryResult ViewExpenseReport::execute()
{
    QueryResult result{ request() };

    return result;
}
