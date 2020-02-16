#ifndef ADDEXPENSETRANSACTION_H
#define ADDEXPENSETRANSACTION_H

#include "expenseexecutor.h"

namespace Utility {
class Client;
class ExpenseTransaction;
}

namespace ExpenseQuery {
class AddExpenseTransaction : public ExpenseExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("add_expense_transaction");

    explicit AddExpenseTransaction(const Utility::ExpenseTransaction &transaction,
                                   QObject *receiver);
    QueryResult execute() override;
};
}

#endif // ADDEXPENSETRANSACTION_H
