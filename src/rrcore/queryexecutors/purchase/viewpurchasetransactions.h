#ifndef VIEWPURCHASETRANSACTIONS_H
#define VIEWPURCHASETRANSACTIONS_H

#include "purchaseexecutor.h"

namespace PurchaseQuery {
class ViewPurchaseTransactions : public PurchaseExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_purchase_transactions");

    explicit ViewPurchaseTransactions(const QDateTime &from,
                                      const QDateTime &to,
                                      bool suspended,
                                      bool archived,
                                      QObject *receiver);
    QueryResult execute() override;
};
}

#endif // VIEWPURCHASETRANSACTIONS_H
