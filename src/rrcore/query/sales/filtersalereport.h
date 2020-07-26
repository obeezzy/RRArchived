#ifndef FILTERSALEREPORT_H
#define FILTERSALEREPORT_H

#include "saleexecutor.h"

namespace Utility {
class FilterCriteria;
class SortCriteria;
class DateTimeSpan;
}  // namespace Utility

namespace Query {
namespace Sales {
class FilterSaleReport : public SaleExecutor
{
    Q_OBJECT
public:
    inline static const QString COMMAND = QStringLiteral("filter_sale_report");

    explicit FilterSaleReport(const Utility::FilterCriteria& filterCriteria,
                              const Utility::SortCriteria& sortCriteria,
                              const Utility::DateTimeSpan& dateTimeSpan,
                              QObject* receiver);
    QueryResult execute() override;
};
}  // namespace Sales
}  // namespace Query

#endif  // FILTERSALEREPORT_H
