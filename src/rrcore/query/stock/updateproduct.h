#ifndef UPDATEPRODUCT_H
#define UPDATEPRODUCT_H

#include "stockexecutor.h"

namespace Utility {
    namespace Stock {
    class Product;
    }
}

namespace Query {
namespace Stock {
class UpdateProduct : public StockExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("update_product");

    explicit UpdateProduct(const Utility::Stock::Product &product,
                                QObject *receiver);
    QueryResult execute() override;
private:
    int addOrUpdateProductCategory();
    void updateProduct(int productCategoryId);
    void updateProductUnit();
};
}
}

Q_DECLARE_LOGGING_CATEGORY(lcupdateproduct);

#endif // UPDATEPRODUCT_H
