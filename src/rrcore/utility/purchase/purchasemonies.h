#ifndef PURCHASEMONIES_H
#define PURCHASEMONIES_H

#include "utility/common/money.h"
#include <QVariantMap>

namespace Utility {
struct PurchaseMonies
{
    Money amountPaid;
    Money balance;
    Money cost;
    Money costPrice;
    Money discount;
    Money retailPrice;
    Money totalCost;
    Money unitPrice;

    explicit PurchaseMonies();
    explicit PurchaseMonies(const QVariantMap &map);

    QVariantMap toVariantMap() const;
};
}

#endif // PURCHASEMONIES_H
