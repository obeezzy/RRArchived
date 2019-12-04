#ifndef REMOVEPURCHASETRANSACTIONITEM_H
#define REMOVEPURCHASETRANSACTIONITEM_H

#include "purchaseexecutor.h"

namespace PurchaseQuery {
class RemovePurchaseTransactionItem : public PurchaseExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("remove_purchase_transaction_item");
    static inline const QString UNDO_COMMAND = QStringLiteral("undo_remove_purchase_transaction_item");

    explicit RemovePurchaseTransactionItem(qint64 transactionId,
                                           int transactionItemId,
                                           QObject *receiver);
    QueryResult execute() override;    
};
}

#endif // REMOVEPURCHASETRANSACTIONITEM_H
