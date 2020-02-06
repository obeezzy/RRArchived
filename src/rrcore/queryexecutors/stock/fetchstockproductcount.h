#ifndef FETCHSTOCKPRODUCTCOUNT_H
#define FETCHSTOCKPRODUCTCOUNT_H

#include "stockexecutor.h"

namespace StockQuery {
class FetchStockProductCount : public StockExecutor
{
public:
    static inline const QString COMMAND = QStringLiteral("fetch_stock_product_count");

    explicit FetchStockProductCount(int categoryId,
                                    QObject *receiver);
    QueryResult execute() override;
};
}

#endif // FETCHSTOCKPRODUCTCOUNT_H
