#ifndef ADDPURCHASETRANSACTION_H
#define ADDPURCHASETRANSACTION_H

#include "purchaseexecutor.h"
#include "utility/purchaseutils.h"
#include "utility/stockutils.h"

namespace PurchaseQuery {
class AddPurchaseTransaction : public PurchaseExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("add_purchase_transaction");
    static inline const QString UNDO_COMMAND = QStringLiteral("undo_add_purchase_transaction");

    explicit AddPurchaseTransaction(qint64 transactionId,
                                    int clientId,
                                    const QString &customerName,
                                    const QString &customerPhoneNumber,
                                    qreal totalCost,
                                    qreal amountPaid,
                                    qreal balance,
                                    bool suspended,
                                    const QDateTime &dueDate,
                                    const QString &action,
                                    const PurchasePaymentList &payments,
                                    const StockItemList &items,
                                    const QString &note,
                                    QObject *receiver);
    QueryResult execute() override;
private:
    QueryResult undoAddPurchaseTransaction();
};
}

#endif // ADDPURCHASETRANSACTION_H
