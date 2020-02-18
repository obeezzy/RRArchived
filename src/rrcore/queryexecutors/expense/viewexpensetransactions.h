#ifndef VIEWEXPENSETRANSACTIONS_H
#define VIEWEXPENSETRANSACTIONS_H

#include "expenseexecutor.h"
#include "utility/common/recordgroup.h"

namespace Utility {
class DateTimeSpan;
}

namespace ExpenseQuery {
class ViewExpenseTransactions : public ExpenseExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_expense_transactions");

    explicit ViewExpenseTransactions(const Utility::DateTimeSpan &dateTimeSpan,
                                     const Utility::RecordGroup::Flags &flags,
                                     QObject *receiver);
    QueryResult execute() override;
};
}

#endif // VIEWEXPENSETRANSACTIONS_H
