#include "filterstockitems.h"

using namespace StockQuery;

FilterStockItems::FilterStockItems(int categoryId,
                                   const QString &filterText,
                                   const QString &filterColumn,
                                   Qt::SortOrder sortOrder,
                                   const QString &sortColumn,
                                   QObject *receiver) :
    StockExecutor(COMMAND, {
                        { "category_id", categoryId },
                        { "filter_text", filterText },
                        { "filter_column", filterColumn },
                        { "sort_order", sortOrder },
                        { "sort_column", sortColumn }
                  }, receiver)
{

}

QueryResult FilterStockItems::execute()
{
    QueryResult result{ request() };

    return result;
}
