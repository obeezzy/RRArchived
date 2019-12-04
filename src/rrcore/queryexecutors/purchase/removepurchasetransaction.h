#ifndef REMOVEPURCHASETRANSACTION_H
#define REMOVEPURCHASETRANSACTION_H

#include "purchaseexecutor.h"
#include "utility/purchaseutils.h"

namespace PurchaseQuery {
class RemovePurchaseTransaction : public PurchaseExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("remove_purchase_transaction");
    static inline const QString UNDO_COMMAND = QStringLiteral("undo_remove_purchase_transaction");

    explicit RemovePurchaseTransaction(qint64 transactionId,
                                       int row,
                                       const PurchaseTransaction &transaction,
                                       QObject *receiver);
    QueryResult execute() override;
private:
    QueryResult removePurchaseTransaction();
    QueryResult undoRemovePurchaseTransaction();
};
}

#endif // REMOVEPURCHASETRANSACTION_H
