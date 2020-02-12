#ifndef VIEWPURCHASEREPORT_H
#define VIEWPURCHASEREPORT_H

#include "purchaseexecutor.h"

class DateTimeSpan;

namespace PurchaseQuery {
class ViewPurchaseReport : public PurchaseExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_purchase_report");

    explicit ViewPurchaseReport(const DateTimeSpan &dateTimeSpan,
                                QObject *receiver);
    QueryResult execute() override;
};
}

#endif // VIEWPURCHASEREPORT_H
