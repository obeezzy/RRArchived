#ifndef VIEWEXPENSETRANSACTIONS_H
#define VIEWEXPENSETRANSACTIONS_H

#include "expenseexecutor.h"
#include "utility/commonutils.h"

namespace ExpenseQuery {
class ViewExpenseTransactions : public ExpenseExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_expense_transactions");

    explicit ViewExpenseTransactions(const DateTimeSpan &dateTimeSpan,
                                     const RecordGroup::Flags &flags,
                                     QObject *receiver);
    QueryResult execute() override;
};
}

#endif // VIEWEXPENSETRANSACTIONS_H
