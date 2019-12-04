#ifndef VIEWPURCHASETRANSACTIONITEMS_H
#define VIEWPURCHASETRANSACTIONITEMS_H

#include "purchaseexecutor.h"

namespace PurchaseQuery {
class ViewPurchaseTransactionItems : public PurchaseExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_purchase_transaction_items");

    explicit ViewPurchaseTransactionItems(qint64 transactionId,
                                          QObject *receiver);
    QueryResult execute() override;
};
}

#endif // VIEWPURCHASETRANSACTIONITEMS_H
