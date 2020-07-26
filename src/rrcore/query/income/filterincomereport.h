#ifndef FILTERINCOMEREPORT_H
#define FILTERINCOMEREPORT_H

#include "incomeexecutor.h"

namespace Utility {
class DateTimeSpan;
class FilterCriteria;
class SortCriteria;
}  // namespace Utility

namespace Query {
namespace Income {
class FilterIncomeReport : public IncomeExecutor
{
    Q_OBJECT
public:
    inline static const QString COMMAND =
        QStringLiteral("filter_income_report");

    explicit FilterIncomeReport(const Utility::FilterCriteria& filterCriteria,
                                const Utility::SortCriteria& sortCriteria,
                                const Utility::DateTimeSpan& dateTimeSpan,
                                QObject* receiver);
    QueryResult execute() override;
};
}  // namespace Income
}  // namespace Query

#endif  // FILTERINCOMEREPORT_H
