#ifndef FILTERINCOMEREPORT_H
#define FILTERINCOMEREPORT_H

#include "incomeexecutor.h"

namespace Utility {
class DateTimeSpan;
class FilterCriteria;
class SortCriteria;
}

namespace IncomeQuery {
class FilterIncomeReport : public IncomeExecutor
{
    Q_OBJECT
public:
    inline static const QString COMMAND = QStringLiteral("filter_income_report");

    explicit FilterIncomeReport(const Utility::FilterCriteria &filterCriteria,
                                const Utility::SortCriteria &sortCriteria,
                                const Utility::DateTimeSpan &dateTimeSpan,
                                QObject *receiver);
    QueryResult execute() override;
};
}

#endif // FILTERINCOMEREPORT_H
