#ifndef ADDSTOCKPRODUCT_H
#define ADDSTOCKPRODUCT_H

#include "stockexecutor.h"
#include "utility/stockutils.h"

namespace StockQuery {
class AddStockProduct : public StockExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("add_stock_product");

    explicit AddStockProduct(const StockProduct &product,
                             QObject *receiver);
    QueryResult execute() override;

private:
};
}

#endif // ADDSTOCKPRODUCT_H
