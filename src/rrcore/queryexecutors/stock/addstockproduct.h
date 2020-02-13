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

    explicit AddStockProduct(const Utility::StockProduct &product,
                             QObject *receiver);
    QueryResult execute() override;
private:
    int addStockProductCategory();
    int addStockProduct(int productCategoryId);
    int addStockProductUnit(int productId);
    void addInitialProductQuantity(int productId,
                                   int productUnitId);
    void addCurrentProductQuantity(int productId,
                                   int productUnitId);
};
}

#endif // ADDSTOCKPRODUCT_H
