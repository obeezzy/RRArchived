#ifndef ADDEXPENSETRANSACTION_H
#define ADDEXPENSETRANSACTION_H

#include "expenseexecutor.h"

namespace Utility {
namespace Expense {
class ExpenseTransaction;
}
}

namespace Query {
    namespace Expense {
    class AddExpenseTransaction : public ExpenseExecutor
    {
        Q_OBJECT
    public:
        static inline const QString COMMAND = QStringLiteral("add_expense_transaction");

        explicit AddExpenseTransaction(const Utility::Expense::ExpenseTransaction &transaction,
                                       QObject *receiver);
        QueryResult execute() override;
    };
}
}

#endif // ADDEXPENSETRANSACTION_H
