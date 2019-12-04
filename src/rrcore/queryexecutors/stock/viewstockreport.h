#ifndef VIEWSTOCKREPORT_H
#define VIEWSTOCKREPORT_H

#include "stockexecutor.h"

namespace StockQuery {
class ViewStockReport : public StockExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_stock_report");

    explicit ViewStockReport(QObject *receiver);
    QueryResult execute() override;
};
}

#endif // VIEWSTOCKREPORT_H
