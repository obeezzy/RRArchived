#ifndef REMOVESALETRANSACTION_H
#define REMOVESALETRANSACTION_H

#include "saleexecutor.h"

namespace SaleQuery {
class RemoveSaleTransaction : public SaleExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("remove_sale_transaction");
    static inline const QString UNDO_COMMAND = QStringLiteral("undo_remove_sale_transaction");

    explicit RemoveSaleTransaction(qint64 transactionId,
                                   QObject *receiver);
    QueryResult execute() override;
};
}

#endif // REMOVESALETRANSACTION_H
