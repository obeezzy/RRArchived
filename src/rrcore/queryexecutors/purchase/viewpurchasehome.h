#ifndef VIEWPURCHASEHOME_H
#define VIEWPURCHASEHOME_H

#include "purchaseexecutor.h"

namespace PurchaseQuery {
class ViewPurchaseHome : public PurchaseExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_purchase_home");

    explicit ViewPurchaseHome(QObject *receiver);
    QueryResult execute() override;
};
}

#endif // VIEWPURCHASEHOME_H
