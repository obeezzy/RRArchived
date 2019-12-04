#ifndef REMOVESALETRANSACTIONITEM_H
#define REMOVESALETRANSACTIONITEM_H

#include "saleexecutor.h"

namespace SaleQuery {
class RemoveSaleTransactionItem : public SaleExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("remove_sale_transaction_item");

    explicit RemoveSaleTransactionItem(qint64 transactionId,
                                       int transactionItemId,
                                       QObject *receiver);
    QueryResult execute() override;
};
}

#endif // REMOVESALETRANSACTIONITEM_H
