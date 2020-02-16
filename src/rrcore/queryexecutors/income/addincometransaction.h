#ifndef ADDINCOMETRANSACTION_H
#define ADDINCOMETRANSACTION_H

#include "incomeexecutor.h"

namespace Utility {
class IncomeTransaction;
}

namespace IncomeQuery {
class AddIncomeTransaction : public IncomeExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("add_income_transaction");

    explicit AddIncomeTransaction(const Utility::IncomeTransaction &transaction,
                                  QObject *receiver);
    QueryResult execute() override;
};
}

#endif // ADDINCOMETRANSACTION_H
