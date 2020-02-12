#ifndef VIEWSALEREPORT_H
#define VIEWSALEREPORT_H

#include "saleexecutor.h"

class DateTimeSpan;

namespace SaleQuery {
class ViewSaleReport : public SaleExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_sale_report");

    explicit ViewSaleReport(const DateTimeSpan &dateTimeSpan,
                            QObject *receiver);
    QueryResult execute() override;
};
}

#endif // VIEWSALEREPORT_H
