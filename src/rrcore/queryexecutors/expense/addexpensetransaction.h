#ifndef ADDEXPENSETRANSACTION_H
#define ADDEXPENSETRANSACTION_H

#include "expenseexecutor.h"

namespace ExpenseQuery {
class AddExpenseTransaction : public ExpenseExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("add_new_expense_transaction");

    explicit AddExpenseTransaction(const QString &clientName,
                                   const QString &purpose,
                                   qreal amount,
                                   const QString &paymentMethod,
                                   QObject *receiver);
    QueryResult execute() override;
};
}

#endif // ADDEXPENSETRANSACTION_H
