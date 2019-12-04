#ifndef VIEWPURCHASEREPORT_H
#define VIEWPURCHASEREPORT_H

#include "purchaseexecutor.h"

namespace PurchaseQuery {
class ViewPurchaseReport : public PurchaseExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_purchase_report");

    explicit ViewPurchaseReport(QObject *receiver);
    QueryResult execute() override;
};
}

#endif // VIEWPURCHASEREPORT_H
