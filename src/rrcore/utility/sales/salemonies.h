#ifndef SALEMONIES_H
#define SALEMONIES_H

#include "utility/common/money.h"

namespace Utility {
    namespace Sales {
    struct SaleMonies
    {
        Money amountPaid;
        Money balance;
        Money cost;
        Money costPrice;
        Money discount;
        Money retailPrice;
        Money totalCost;
        Money unitPrice;

        explicit SaleMonies();
        explicit SaleMonies(const QVariantMap &map);

        QVariantMap toVariantMap() const;

        friend QDebug operator<<(QDebug debug, const SaleMonies &monies)
        {
            debug.nospace() << "Sales::Monies(";
            if (!monies.amountPaid.isZero())
                debug.nospace() << ", amountPaid=" << monies.amountPaid;
            if (!monies.balance.isZero())
                debug.nospace() << ", balance=" << monies.balance;
            if (!monies.cost.isZero())
                debug.nospace() << ", cost=" << monies.cost;
            if (!monies.costPrice.isZero())
                debug << ", costPrice=" << monies.costPrice;
            if (!monies.discount.isZero())
                debug << ", discount=" << monies.discount;
            if (!monies.retailPrice.isZero())
                debug << ", retailPrice=" << monies.retailPrice;
            if (!monies.totalCost.isZero())
                debug << ", totalCost=" << monies.totalCost;
            if (!monies.unitPrice.isZero())
                debug << ", unitPrice=" << monies.unitPrice;
            debug << ")";

            return debug.nospace();
        }
    };
}
}

#endif // SALEMONIES_H
