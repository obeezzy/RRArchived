#ifndef VIEWDEBTPAYMENTS_H
#define VIEWDEBTPAYMENTS_H

#include "debtorexecutor.h"

namespace Query {
    namespace Debtor {
    class ViewDebtPayments : public DebtorExecutor
    {
        Q_OBJECT
    public:
        static inline const QString COMMAND = QStringLiteral("view_debt_payments");

        explicit ViewDebtPayments(int debtTransactionId,
                                  QObject *receiver);
        QueryResult execute() override;
    private:
        void fetchDebtPayments(QVariantList &debtPayments);
    };
}
}

#endif // VIEWDEBTPAYMENTS_H
