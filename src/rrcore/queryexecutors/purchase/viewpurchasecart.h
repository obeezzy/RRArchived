#ifndef VIEWPURCHASECART_H
#define VIEWPURCHASECART_H

#include "purchaseexecutor.h"

namespace PurchaseQuery {
class ViewPurchaseCart : public PurchaseExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_purchase_cart");

    explicit ViewPurchaseCart(int transactionId,
                              QObject *receiver);
    QueryResult execute() override;
};
}

#endif // VIEWPURCHASECART_H
