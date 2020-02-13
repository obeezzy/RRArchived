#ifndef FILTERSTOCKREPORT_H
#define FILTERSTOCKREPORT_H

#include "stockexecutor.h"

namespace Utility {
class FilterCriteria;
class SortCriteria;
class DateTimeSpan;
}

class FilterStockReport : public StockExecutor
{
    Q_OBJECT
public:
    inline static const QString COMMAND = QStringLiteral("filter_stock_report");

    explicit FilterStockReport(const Utility::FilterCriteria &filterCriteria,
                               const Utility::SortCriteria &sortCriteria,
                               const Utility::DateTimeSpan &dateTimeSpan,
                               QObject *receiver);
    QueryResult execute() override;
};

#endif // FILTERSTOCKREPORT_H
