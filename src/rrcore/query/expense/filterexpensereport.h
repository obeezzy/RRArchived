#ifndef FILTEREXPENSEREPORT_H
#define FILTEREXPENSEREPORT_H

#include "expenseexecutor.h"

namespace Utility {
class FilterCriteria;
class SortCriteria;
class DateTimeSpan;
}  // namespace Utility

class FilterExpenseReport : public ExpenseExecutor
{
    Q_OBJECT
public:
    inline static const QString COMMAND =
        QStringLiteral("filter_expense_report");

    explicit FilterExpenseReport(const Utility::FilterCriteria& filterCriteria,
                                 const Utility::SortCriteria& sortCriteria,
                                 const Utility::DateTimeSpan& dateTimeSpan,
                                 QObject* receiver);
    QueryResult execute() override;
};

#endif  // FILTEREXPENSEREPORT_H
