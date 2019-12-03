#ifndef VIEWSALETRANSACTIONITEMS_H
#define VIEWSALETRANSACTIONITEMS_H

#include "saleexecutor.h"

namespace SaleQuery {
class ViewSaleTransactionItems : public SaleExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_sale_transaction_items");

    explicit ViewSaleTransactionItems(qint64 transactionId,
                                      QObject *receiver);
    QueryResult execute() override;
};
}

#endif // VIEWSALETRANSACTIONITEMS_H
