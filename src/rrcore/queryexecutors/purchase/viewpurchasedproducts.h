#ifndef VIEWPURCHASEDPRODUCTS_H
#define VIEWPURCHASEDPRODUCTS_H

#include "purchaseexecutor.h"

namespace PurchaseQuery {
class ViewPurchasedProducts : public PurchaseExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_purchased_products");

    explicit ViewPurchasedProducts(int transactionId,
                                   QObject *receiver);
    QueryResult execute() override;
};
}

#endif // VIEWPURCHASEDPRODUCTS_H
