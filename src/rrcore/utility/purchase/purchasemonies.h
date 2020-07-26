#ifndef PURCHASEMONIES_H
#define PURCHASEMONIES_H

#include <QVariantMap>
#include "utility/common/money.h"

namespace Utility {
namespace Purchase {
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
    explicit PurchaseMonies(const QVariantMap& map);

    QVariantMap toVariantMap() const;
};
}  // namespace Purchase
}  // namespace Utility

#endif  // PURCHASEMONIES_H
