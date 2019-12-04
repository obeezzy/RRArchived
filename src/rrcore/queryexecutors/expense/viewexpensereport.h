#ifndef VIEWEXPENSEREPORT_H
#define VIEWEXPENSEREPORT_H

#include "expenseexecutor.h"

namespace ExpenseQuery {
class ViewExpenseReport : public ExpenseExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_expense_report");

    explicit ViewExpenseReport(QObject *receiver);
    QueryResult execute() override;
};
}

#endif // VIEWEXPENSEREPORT_H
