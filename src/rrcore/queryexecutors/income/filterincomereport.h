#ifndef FILTERINCOMEREPORT_H
#define FILTERINCOMEREPORT_H

#include "queryexecutors/income/incomeexecutor.h"

class DateTimeSpan;
class FilterCriteria;
class SortCriteria;

namespace IncomeQuery {
class FilterIncomeReport : public IncomeExecutor
{
    Q_OBJECT
public:
    inline static const QString COMMAND = QStringLiteral("filter_income_report");

    explicit FilterIncomeReport(const FilterCriteria &filterCriteria,
                                const SortCriteria &sortCriteria,
                                const DateTimeSpan &dateTimeSpan,
                                QObject *receiver);
    QueryResult execute() override;
};
}

#endif // FILTERINCOMEREPORT_H
