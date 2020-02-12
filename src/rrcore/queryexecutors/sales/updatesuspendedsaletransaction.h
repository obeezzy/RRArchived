#ifndef UPDATESUSPENDEDSALETRANSACTION_H
#define UPDATESUSPENDEDSALETRANSACTION_H

#include "saleexecutor.h"

class SaleTransaction;

namespace SaleQuery {
class UpdateSuspendedSaleTransaction : public SaleExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("update_suspended_sale_transaction");

    explicit UpdateSuspendedSaleTransaction(const SaleTransaction &transaction,
                                            QObject *receiver);
    QueryResult execute() override;
private:
    void affirmTransactionHasBeenSuspended();
    void archiveSaleTransaction();
};
}

#endif // UPDATESUSPENDEDSALETRANSACTION_H
