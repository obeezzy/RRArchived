#ifndef DEBTORMONIES_H
#define DEBTORMONIES_H

#include "utility/common/money.h"
#include <QVariant>

namespace Utility {
    namespace Debtor {
    struct DebtorMonies
    {
        Money debt;
        Money amountPaid;
        Money balance;

        explicit DebtorMonies() = default;
        explicit DebtorMonies(const QVariantMap &map);

        QVariantMap toVariantMap() const;

        friend QDebug operator<<(QDebug debug, const DebtorMonies &monies)
        {
            debug.nospace() << "Utility::Debtor::Monies(";
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
}

#endif // DEBTORMONIES_H
