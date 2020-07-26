#ifndef UPDATESUSPENDEDPURCHASETRANSACTION_H
#define UPDATESUSPENDEDPURCHASETRANSACTION_H

#include "purchaseexecutor.h"

namespace Utility {
namespace Purchase {
class PurchaseTransaction;
}
}

namespace Query {
    namespace Purchase {
    class UpdateSuspendedPurchaseTransaction : public PurchaseExecutor
    {
        Q_OBJECT
    public:
        static inline const QString COMMAND = QStringLiteral("update_suspended_purchase_transaction");

        explicit UpdateSuspendedPurchaseTransaction(const Utility::Purchase::PurchaseTransaction &transaction,
                                                    QObject *receiver);
        QueryResult execute() override;
    private:
        void affirmTransactionHasBeenSuspended();
        void archivePurchaseTransaction();
    };
}
}

#endif // UPDATESUSPENDEDPURCHASETRANSACTION_H
