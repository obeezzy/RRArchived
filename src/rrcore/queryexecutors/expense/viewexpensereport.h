#ifndef VIEWEXPENSEREPORT_H
#define VIEWEXPENSEREPORT_H

#include "expenseexecutor.h"

class DateTimeSpan;

namespace ExpenseQuery {
class ViewExpenseReport : public ExpenseExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_expense_report");

    explicit ViewExpenseReport(const DateTimeSpan &dateTimeSpan,
                               QObject *receiver);
    QueryResult execute() override;
};
}

#endif // VIEWEXPENSEREPORT_H
