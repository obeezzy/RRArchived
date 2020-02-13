#ifndef FILTERSTOCKPRODUCTS_H
#define FILTERSTOCKPRODUCTS_H

#include "stockexecutor.h"
#include "utility/commonutils.h"

namespace StockQuery {
class FilterStockProducts : public StockExecutor
{
    Q_OBJECT
public:
    static inline QString COMMAND = QStringLiteral("filter_stock_products");

    explicit FilterStockProducts(const Utility::FilterCriteria &filterCriteria,
                                 const Utility::SortCriteria &sortCriteria,
                                 int categoryId,
                                 QObject *receiver);
    QueryResult execute() override;
};
}

#endif // FILTERSTOCKPRODUCTS_H
