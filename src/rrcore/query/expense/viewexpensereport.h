#ifndef VIEWEXPENSEREPORT_H
#define VIEWEXPENSEREPORT_H

#include "expenseexecutor.h"

namespace Utility {
class DateTimeSpan;
}

namespace Query {
namespace Expense {
class ViewExpenseReport : public ExpenseExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_expense_report");

    explicit ViewExpenseReport(const Utility::DateTimeSpan& dateTimeSpan,
                               QObject* receiver);
    QueryResult execute() override;
};
}  // namespace Expense
}  // namespace Query

#endif  // VIEWEXPENSEREPORT_H
