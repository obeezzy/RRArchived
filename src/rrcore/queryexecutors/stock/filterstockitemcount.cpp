#include "filterstockitemcount.h"

using namespace StockQuery;

FilterStockItemCount::FilterStockItemCount(int categoryId,
                                           const QString &filterText,
                                           const QString &filterColumn,
                                           QObject *receiver) :
    StockExecutor(COMMAND, {
                        { "category_id", categoryId },
                        { "filter_text", filterText },
                        { "filter_column", filterColumn }
                  }, receiver)
{

}

QueryResult FilterStockItemCount::execute()
{
    QueryResult result{ request() };

    return result;
}
