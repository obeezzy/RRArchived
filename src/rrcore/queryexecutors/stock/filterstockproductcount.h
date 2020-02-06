#ifndef FILTERSTOCKPRODUCTCOUNT_H
#define FILTERSTOCKPRODUCTCOUNT_H

#include "stockexecutor.h"

namespace StockQuery {
class FilterStockProductCount : public StockExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("filter_stock_product_count");

    explicit FilterStockProductCount(int categoryId,
                                     const QString &filterText,
                                     const QString &filterColumn,
                                     QObject *receiver);
    QueryResult execute() override;
};
}

#endif // FILTERSTOCKPRODUCTCOUNT_H
