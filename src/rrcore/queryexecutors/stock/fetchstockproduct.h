#ifndef FETCHSTOCKPRODUCT_H
#define FETCHSTOCKPRODUCT_H

#include "stockexecutor.h"

namespace StockQuery {
class FetchStockProduct : public StockExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("fetch_stock_product");

    explicit FetchStockProduct(int productId,
                               QObject *receiver);
    QueryResult execute() override;
};
}

#endif // FETCHSTOCKPRODUCT_H
