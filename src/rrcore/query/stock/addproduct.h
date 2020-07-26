#ifndef ADDSTOCKPRODUCT_H
#define ADDSTOCKPRODUCT_H

#include "stockexecutor.h"
#include "utility/stock/product.h"

namespace Query {
namespace Stock {
class AddProduct : public StockExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("add_product");

    explicit AddProduct(const Utility::Stock::Product& product,
                        QObject* receiver);
    QueryResult execute() override;

private:
    int addProductCategory();
    int addProduct(int productCategoryId);
    int addProductUnit(int productId);
    void addProductQuantity(int productId);
};
}  // namespace Stock
}  // namespace Query

#endif  // ADDSTOCKPRODUCT_H
