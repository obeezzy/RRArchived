#ifndef VIEWSALEREPORT_H
#define VIEWSALEREPORT_H

#include "saleexecutor.h"

namespace Utility {
class DateTimeSpan;
}

namespace Query {
namespace Sales {
class ViewSaleReport : public SaleExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_sale_report");

    explicit ViewSaleReport(const Utility::DateTimeSpan& dateTimeSpan,
                            QObject* receiver);
    QueryResult execute() override;
};
}  // namespace Sales
}  // namespace Query

#endif  // VIEWSALEREPORT_H
