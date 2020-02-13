#ifndef FILTERSTOCKPRODUCTCOUNT_H
#define FILTERSTOCKPRODUCTCOUNT_H

#include "stockexecutor.h"

namespace Utility {
class FilterCriteria;
}

namespace StockQuery {
class FilterStockProductCount : public StockExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("filter_stock_product_count");

    explicit FilterStockProductCount(const Utility::FilterCriteria &filterCriteria,
                                     int categoryId,
                                     QObject *receiver);
    QueryResult execute() override;
};
}

#endif // FILTERSTOCKPRODUCTCOUNT_H
