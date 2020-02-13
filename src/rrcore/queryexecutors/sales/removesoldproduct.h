#ifndef REMOVESOLDPRODUCT_H
#define REMOVESOLDPRODUCT_H

#include "saleexecutor.h"

namespace Utility {
class SoldProduct;
}

namespace SaleQuery {
class RemoveSoldProduct : public SaleExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("remove_sold_product");

    explicit RemoveSoldProduct(const Utility::SoldProduct &product,
                               QObject *receiver);
    QueryResult execute() override;
};
}

#endif // REMOVESOLDPRODUCT_H
