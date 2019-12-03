#include "viewstockreport.h"

using namespace StockQuery;

ViewStockReport::ViewStockReport(QObject *receiver) :
    StockExecutor(COMMAND, {}, receiver)
{

}

QueryResult ViewStockReport::execute()
{
    QueryResult result{ request() };

    return result;
}
