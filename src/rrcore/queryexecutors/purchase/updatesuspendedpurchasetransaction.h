#ifndef UPDATESUSPENDEDPURCHASETRANSACTION_H
#define UPDATESUSPENDEDPURCHASETRANSACTION_H

#include "purchaseexecutor.h"
#include "utility/purchaseutils.h"

namespace PurchaseQuery {
class UpdateSuspendedPurchaseTransaction : public PurchaseExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("update_suspended_purchase_transaction");

    explicit UpdateSuspendedPurchaseTransaction(qint64 transactionId,
                                                int clientId,
                                                const QString &customerName,
                                                const QString &customerPhoneNumber,
                                                qreal totalCost,
                                                qreal amountPaid,
                                                qreal balance,
                                                bool suspended,
                                                const PurchaseCartProductList &products,
                                                const Note &note,
                                                QObject *receiver);
    QueryResult execute() override;
};
}

#endif // UPDATESUSPENDEDPURCHASETRANSACTION_H
