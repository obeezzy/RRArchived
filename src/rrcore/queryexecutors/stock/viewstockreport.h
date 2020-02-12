#ifndef VIEWSTOCKREPORT_H
#define VIEWSTOCKREPORT_H

#include "stockexecutor.h"

class DateTimeSpan;

namespace StockQuery {
class ViewStockReport : public StockExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_stock_report");

    explicit ViewStockReport(const DateTimeSpan &dateTimeSpan, QObject *receiver);
    QueryResult execute() override;
};
}

#endif // VIEWSTOCKREPORT_H
