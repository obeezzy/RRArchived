#ifndef UPDATESUSPENDEDPURCHASETRANSACTION_H
#define UPDATESUSPENDEDPURCHASETRANSACTION_H

#include "purchaseexecutor.h"

namespace Utility {
class PurchaseTransaction;
}

namespace PurchaseQuery {
class UpdateSuspendedPurchaseTransaction : public PurchaseExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("update_suspended_purchase_transaction");

    explicit UpdateSuspendedPurchaseTransaction(const Utility::PurchaseTransaction &transaction,
                                                QObject *receiver);
    QueryResult execute() override;
private:
    void affirmTransactionHasBeenSuspended();
    void archivePurchaseTransaction();
};
}

#endif // UPDATESUSPENDEDPURCHASETRANSACTION_H
