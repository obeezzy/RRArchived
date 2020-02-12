#ifndef VIEWPURCHASEHOME_H
#define VIEWPURCHASEHOME_H

#include "purchaseexecutor.h"

class DateTimeSpan;

namespace PurchaseQuery {
class ViewPurchaseHome : public PurchaseExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_purchase_home");

    explicit ViewPurchaseHome(const DateTimeSpan &dateTimeSpan,
                              QObject *receiver);
    QueryResult execute() override;
private:
    void fetchLastPurchasedProducts(QVariantList &homeRecords);
};
}

#endif // VIEWPURCHASEHOME_H
