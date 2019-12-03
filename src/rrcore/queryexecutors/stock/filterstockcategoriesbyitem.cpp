#include "filterstockcategoriesbyitem.h"

using namespace StockQuery;

FilterStockCategoriesByItem::FilterStockCategoriesByItem(const QString &itemFilterText,
                                                         Qt::SortOrder sortOrder,
                                                         bool archived,
                                                         QObject *receiver) :
    StockExecutor(COMMAND, {
                    { "filter_text", itemFilterText },
                    { "sort_order", sortOrder == Qt::AscendingOrder ? "ascending" : "descending" },
                    { "archived", archived }
                  }, receiver)
{

}

QueryResult StockQuery::FilterStockCategoriesByItem::execute()
{
    QueryResult result{ request() };

    return result;
}
