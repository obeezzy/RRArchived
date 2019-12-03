#include "viewstockitemdetails.h"

using namespace StockQuery;

ViewStockItemDetails::ViewStockItemDetails(int itemId, QObject *receiver) :
    StockExecutor(COMMAND, {
                        { "item_id", itemId }
                  }, receiver)
{

}

QueryResult ViewStockItemDetails::execute()
{
    QueryResult result{ request() };

    return result;
}
