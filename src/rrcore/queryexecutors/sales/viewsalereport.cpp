#include "viewsalereport.h"

using namespace SaleQuery;

ViewSaleReport::ViewSaleReport(QObject *receiver) :
    SaleExecutor(COMMAND, {}, receiver)
{

}

QueryResult ViewSaleReport::execute()
{
    QueryResult result{ request() };

    return result;
}
