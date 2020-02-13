#ifndef VIEWDEBTTRANSACTIONS_H
#define VIEWDEBTTRANSACTIONS_H

#include "debtorexecutor.h"

namespace Utility {
class Debtor;
}

namespace DebtorQuery {
class ViewDebtTransactions : public DebtorExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_debt_transactions");

    explicit ViewDebtTransactions(const Utility::Debtor &debtor,
                                  QObject *receiver);
    QueryResult execute() override;
private:
    void fetchDebtor(QVariantMap &debtor);
    void fetchDebtTransactions(QVariantList &debtTransactions);
};
}

#endif // VIEWDEBTTRANSACTIONS_H
