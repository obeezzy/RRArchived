#include "filterstockcategories.h"

using namespace StockQuery;

FilterStockCategories::FilterStockCategories(const QString &filterText,
                                             Qt::SortOrder sortOrder,
                                             bool archived,
                                             QObject *receiver) :
    StockExecutor(COMMAND, {
                    { "filter_text", filterText },
                    { "sort_order", sortOrder == Qt::AscendingOrder ? "ascending" : "descending" },
                    { "archived", archived }
                  }, receiver)
{

}

QueryResult FilterStockCategories::execute()
{
    QueryResult result{ request() };

    return result;
}
