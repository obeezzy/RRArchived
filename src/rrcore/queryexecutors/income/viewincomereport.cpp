#include "viewincomereport.h"

using namespace IncomeQuery;

ViewIncomeReport::ViewIncomeReport(QObject *receiver) :
    IncomeExecutor(COMMAND, {}, receiver)
{

}

QueryResult IncomeQuery::ViewIncomeReport::execute()
{
    QueryResult result{ request() };

    return result;
}
