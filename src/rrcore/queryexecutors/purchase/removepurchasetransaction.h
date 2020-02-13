#ifndef REMOVEPURCHASETRANSACTION_H
#define REMOVEPURCHASETRANSACTION_H

#include "purchaseexecutor.h"

namespace Utility {
class PurchaseTransaction;
}

namespace PurchaseQuery {
class RemovePurchaseTransaction : public PurchaseExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("remove_purchase_transaction");
    static inline const QString UNDO_COMMAND = QStringLiteral("undo_remove_purchase_transaction");

    explicit RemovePurchaseTransaction(const Utility::PurchaseTransaction &transaction,
                                       QObject *receiver);
    QueryResult execute() override;
private:
    QueryResult removePurchaseTransaction();
    QueryResult undoRemovePurchaseTransaction();

    void archivePurchaseTransaction();
    void archiveDebtTransaction();
    void archiveCreditTransaction();
    void revertProductQuantityUpdate();
};
}

#endif // REMOVEPURCHASETRANSACTION_H
