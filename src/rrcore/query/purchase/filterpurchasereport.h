#ifndef FILTERPURCHASEREPORT_H
#define FILTERPURCHASEREPORT_H

#include "purchaseexecutor.h"

namespace Utility {
class FilterCriteria;
class SortCriteria;
class DateTimeSpan;
}  // namespace Utility

namespace Query {
namespace Purchase {
class FilterPurchaseReport : public PurchaseExecutor
{
    Q_OBJECT
public:
    inline static const QString COMMAND =
        QStringLiteral("filter_purchase_report");

    explicit FilterPurchaseReport(const Utility::FilterCriteria& filterCriteria,
                                  const Utility::SortCriteria& sortCriteria,
                                  const Utility::DateTimeSpan& dateTimeSpan,
                                  QObject* receiver);
    QueryResult execute() override;
};
}  // namespace Purchase
}  // namespace Query

#endif  // FILTERPURCHASEREPORT_H
