#include "viewstockitemcount.h"

using namespace StockQuery;

ViewStockItemCount::ViewStockItemCount(QObject *receiver) :
    StockExecutor(COMMAND, { }, receiver)
{

}

QueryResult ViewStockItemCount::execute()
{
    QueryResult result{ request() };

    return result;
}
