#ifndef ADDPURCHASETRANSACTION_H
#define ADDPURCHASETRANSACTION_H

#include "purchaseexecutor.h"

namespace Utility {
class PurchaseTransaction;
}

namespace PurchaseQuery {
class AddPurchaseTransaction : public PurchaseExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("add_purchase_transaction");
    static inline const QString UNDO_COMMAND = QStringLiteral("undo_add_purchase_transaction");

    explicit AddPurchaseTransaction(const Utility::PurchaseTransaction &transaction,
                                    QObject *receiver);
    QueryResult execute() override;
private:
    QueryResult undoAddPurchaseTransaction();

    void archivePurchaseTransaction(int transactionId);
    void archiveDebtTransaction(const QString &transactionTable,
                                int transactionId);
    void archiveCreditTransaction(const QString &transactionTable,
                                  int transactionId);
    void revertProductQuantityUpdate(int transactionId);
};
}

#endif // ADDPURCHASETRANSACTION_H
