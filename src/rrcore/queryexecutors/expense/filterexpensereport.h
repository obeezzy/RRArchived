#ifndef FILTEREXPENSEREPORT_H
#define FILTEREXPENSEREPORT_H

#include "expenseexecutor.h"

class FilterCriteria;
class SortCriteria;
class DateTimeSpan;

class FilterExpenseReport : public ExpenseExecutor
{
    Q_OBJECT
public:
    inline static const QString COMMAND = QStringLiteral("filter_expense_report");

    explicit FilterExpenseReport(const FilterCriteria &filterCriteria,
                                 const SortCriteria &sortCriteria,
                                 const DateTimeSpan &dateTimeSpan,
                                 QObject *receiver);
    QueryResult execute() override;
};

#endif // FILTEREXPENSEREPORT_H
