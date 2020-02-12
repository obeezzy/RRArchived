#ifndef VIEWSALETRANSACTIONS_H
#define VIEWSALETRANSACTIONS_H

#include "saleexecutor.h"
#include "utility/commonutils.h"

namespace SaleQuery {
class ViewSaleTransactions : public SaleExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_sale_transactions");

    explicit ViewSaleTransactions(const DateTimeSpan &dateTimeSpan,
                                  const RecordGroup::Flags &RecordGroups,
                                  QObject *receiver);
    QueryResult execute() override;
};
}

#endif // VIEWSALETRANSACTIONS_H
