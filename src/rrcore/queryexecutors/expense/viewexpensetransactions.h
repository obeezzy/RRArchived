#ifndef VIEWEXPENSETRANSACTIONS_H
#define VIEWEXPENSETRANSACTIONS_H

#include "expenseexecutor.h"

namespace ExpenseQuery {
class ViewExpenseTransactions : public ExpenseExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_expense_transactions");

    explicit ViewExpenseTransactions(const QDateTime &from,
                                     const QDateTime &to,
                                     bool archived,
                                     QObject *receiver);
    QueryResult execute() override;
};
}

#endif // VIEWEXPENSETRANSACTIONS_H
