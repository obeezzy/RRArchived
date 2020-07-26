#ifndef UPDATESUSPENDEDSALETRANSACTION_H
#define UPDATESUSPENDEDSALETRANSACTION_H

#include "saleexecutor.h"

namespace Utility {
namespace Sales {
class SaleTransaction;
}
}  // namespace Utility

namespace Query {
namespace Sales {
class UpdateSuspendedSaleTransaction : public SaleExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND =
        QStringLiteral("update_suspended_sale_transaction");

    explicit UpdateSuspendedSaleTransaction(
        const Utility::Sales::SaleTransaction& transaction, QObject* receiver);
    QueryResult execute() override;

private:
    void affirmTransactionHasBeenSuspended();
    void archiveSaleTransaction();
};
}  // namespace Sales
}  // namespace Query

#endif  // UPDATESUSPENDEDSALETRANSACTION_H
