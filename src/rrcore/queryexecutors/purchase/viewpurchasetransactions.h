#ifndef VIEWPURCHASETRANSACTIONS_H
#define VIEWPURCHASETRANSACTIONS_H

#include "purchaseexecutor.h"
#include "utility/commonutils.h"

namespace PurchaseQuery {
class ViewPurchaseTransactions : public PurchaseExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_purchase_transactions");

    explicit ViewPurchaseTransactions(const Utility::DateTimeSpan &dateTimeSpan,
                                      const Utility::RecordGroup::Flags &flags,
                                      QObject *receiver);
    QueryResult execute() override;
};
}

#endif // VIEWPURCHASETRANSACTIONS_H
