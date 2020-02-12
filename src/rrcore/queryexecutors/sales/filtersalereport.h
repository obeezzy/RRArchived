#ifndef FILTERSALEREPORT_H
#define FILTERSALEREPORT_H

#include "saleexecutor.h"

class FilterCriteria;
class SortCriteria;
class DateTimeSpan;

class FilterSaleReport : public SaleExecutor
{
    Q_OBJECT
public:
    inline static const QString COMMAND = QStringLiteral("filter_sale_report");

    explicit FilterSaleReport(const FilterCriteria &filterCriteria,
                              const SortCriteria &sortCriteria,
                              const DateTimeSpan &dateTimeSpan,
                              QObject *receiver);
    QueryResult execute() override;
};

#endif // FILTERSALEREPORT_H
