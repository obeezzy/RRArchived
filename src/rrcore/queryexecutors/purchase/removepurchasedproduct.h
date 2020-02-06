#ifndef REMOVEPURCHASETRANSACTIONPRODUCT_H
#define REMOVEPURCHASETRANSACTIONPRODUCT_H

#include "purchaseexecutor.h"

namespace PurchaseQuery {
class RemovePurchasedProduct : public PurchaseExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("remove_purchased_product");
    static inline const QString UNDO_COMMAND = QStringLiteral("undo_remove_purchased_product");

    explicit RemovePurchasedProduct(qint64 transactionId,
                                    int purchasedProductId,
                                    QObject *receiver);
    QueryResult execute() override;
};
}

#endif // REMOVEPURCHASETRANSACTIONPRODUCT_H
