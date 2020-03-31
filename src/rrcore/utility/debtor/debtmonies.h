#ifndef DEBTMONIES_H
#define DEBTMONIES_H

#include "utility/common/money.h"
#include <QVariant>

namespace Utility {
struct DebtMonies
{
    Money debt;
    Money amountPaid;
    Money balance;

    explicit DebtMonies() = default;
    explicit DebtMonies(const QVariantMap &map);

    QVariantMap toVariantMap() const;

    friend QDebug operator<<(QDebug debug, const DebtMonies &monies)
    {
        debug.nospace() << "DebtMonies(";
        if (!monies.debt.isZero())
            debug << ", debt=" << monies.debt;
        if (!monies.amountPaid.isZero())
            debug << ", amountPaid=" << monies.amountPaid;
        if (!monies.balance.isZero())
            debug << ", balance=" << monies.balance;
        debug << ")";

        return debug.space();
    }
};
}

#endif // DEBTMONIES_H
