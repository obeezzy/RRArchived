#ifndef REMOVEPURCHASEDPRODUCT_H
#define REMOVEPURCHASEDPRODUCT_H

#include "purchaseexecutor.h"

namespace Utility {
namespace Purchase {
class PurchasedProduct;
}
}  // namespace Utility

namespace Query {
namespace Purchase {
class RemovePurchasedProduct : public PurchaseExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND =
        QStringLiteral("remove_purchased_product");
    static inline const QString UNDO_COMMAND =
        QStringLiteral("undo_remove_purchased_product");

    explicit RemovePurchasedProduct(
        const Utility::Purchase::PurchasedProduct& product, QObject* receiver);
    QueryResult execute() override;
};
}  // namespace Purchase
}  // namespace Query

#endif  // REMOVEPURCHASEDPRODUCT_H
