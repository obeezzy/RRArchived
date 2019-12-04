#ifndef ADDINCOMETRANSACTION_H
#define ADDINCOMETRANSACTION_H

#include "incomeexecutor.h"

namespace IncomeQuery {
class AddIncomeTransaction : public IncomeExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("add_new_income_transaction");

    explicit AddIncomeTransaction(const QString &clientName,
                                  const QString &purpose,
                                  qreal amount,
                                  const QString &paymentMethod,
                                  QObject *receiver);
    QueryResult execute() override;
};
}

#endif // ADDINCOMETRANSACTION_H
