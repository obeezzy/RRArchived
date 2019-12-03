#include "viewstockitems.h"

using namespace StockQuery;

ViewStockItems::ViewStockItems(int categoryId,
                               Qt::SortOrder sortOrder,
                               QObject *receiver) :
    StockExecutor(COMMAND, {
                        { "category_id", categoryId },
                        { "sort_order", sortOrder == Qt::AscendingOrder ? "ascending" : "descending" }
                  }, receiver)
{

}

QueryResult ViewStockItems::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    return result;
}
