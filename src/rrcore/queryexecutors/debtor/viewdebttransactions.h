#ifndef VIEWDEBTTRANSACTIONS_H
#define VIEWDEBTTRANSACTIONS_H

#include "debtorexecutor.h"

namespace DebtorQuery {
class ViewDebtTransactions : public DebtorExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_debt_transactions");

    explicit ViewDebtTransactions(int debtorId,
                                  QObject *receiver);
    QueryResult execute() override;
};
}

#endif // VIEWDEBTTRANSACTIONS_H
