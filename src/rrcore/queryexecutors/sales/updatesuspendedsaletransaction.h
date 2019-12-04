#ifndef UPDATESUSPENDEDSALETRANSACTION_H
#define UPDATESUSPENDEDSALETRANSACTION_H

#include "saleexecutor.h"

namespace SaleQuery {
class UpdateSuspendedSaleTransaction : public SaleExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("update_suspended_sale_transaction");

    explicit UpdateSuspendedSaleTransaction(qint64 transactionId,
                                            const QString &customerName,
                                            int clientId,
                                            const QString &customerPhoneNumber,
                                            qreal totalCost,
                                            qreal amountPaid,
                                            qreal balance,
                                            const QString &note,
                                            bool suspended,
                                            QObject *receiver);
    QueryResult execute() override;
};
}

#endif // UPDATESUSPENDEDSALETRANSACTION_H
