#include "viewstockcategories.h"

using namespace StockQuery;

ViewStockCategories::ViewStockCategories(Qt::SortOrder sortOrder,
                                         bool archived,
                                         QObject *receiver) :
    StockExecutor(COMMAND, {
                    { "sort_order", sortOrder == Qt::AscendingOrder ? "ascending" : "descending" },
                    { "archived", archived }
                  }, receiver)
{

}

QueryResult ViewStockCategories::execute()
{
    QueryResult result{ request() };

    return result;
}
