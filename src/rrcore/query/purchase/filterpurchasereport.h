#ifndef FILTERPURCHASEREPORT_H
#define FILTERPURCHASEREPORT_H

#include "purchaseexecutor.h"

namespace Utility {
class FilterCriteria;
class SortCriteria;
class DateTimeSpan;
}

namespace Query {
    namespace Purchase {
    class FilterPurchaseReport : public PurchaseExecutor
    {
        Q_OBJECT
    public:
        inline static const QString COMMAND = QStringLiteral("filter_purchase_report");

        explicit FilterPurchaseReport(const Utility::FilterCriteria &filterCriteria,
                                      const Utility::SortCriteria &sortCriteria,
                                      const Utility::DateTimeSpan &dateTimeSpan,
                                      QObject *receiver);
        QueryResult execute() override;
    };
}
}

#endif // FILTERPURCHASEREPORT_H
