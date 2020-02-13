#ifndef UPDATESTOCKPRODUCT_H
#define UPDATESTOCKPRODUCT_H

#include "stockexecutor.h"

namespace Utility {
class StockProduct;
}

namespace StockQuery {
class UpdateStockProduct : public StockExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("update_stock_product");

    explicit UpdateStockProduct(const Utility::StockProduct &product,
                                QObject *receiver);
    QueryResult execute() override;
private:
    int addOrUpdateStockProductCategory();
    void updateStockProduct(int productCategoryId);
    void updateStockProductUnit();
};
}

Q_DECLARE_LOGGING_CATEGORY(lcupdatestockproduct);

#endif // UPDATESTOCKPRODUCT_H
