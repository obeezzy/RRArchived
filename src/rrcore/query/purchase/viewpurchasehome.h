#ifndef VIEWPURCHASEHOME_H
#define VIEWPURCHASEHOME_H

#include "purchaseexecutor.h"

namespace Utility {
class DateTimeSpan;
}

namespace Query {
namespace Purchase {
class ViewPurchaseHome : public PurchaseExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_purchase_home");

    explicit ViewPurchaseHome(const Utility::DateTimeSpan& dateTimeSpan,
                              QObject* receiver);
    QueryResult execute() override;

private:
    void fetchLastPurchasedProducts(QVariantList& homeRecords);
};
}  // namespace Purchase
}  // namespace Query

#endif  // VIEWPURCHASEHOME_H
