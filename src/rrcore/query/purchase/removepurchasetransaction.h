#ifndef REMOVEPURCHASETRANSACTION_H
#define REMOVEPURCHASETRANSACTION_H

#include "purchaseexecutor.h"

namespace Utility {
namespace Purchase {
class PurchaseTransaction;
}
}

namespace Query {
    namespace Purchase {
    class RemovePurchaseTransaction : public PurchaseExecutor
    {
        Q_OBJECT
    public:
        static inline const QString COMMAND = QStringLiteral("remove_purchase_transaction");
        static inline const QString UNDO_COMMAND = QStringLiteral("undo_remove_purchase_transaction");

        explicit RemovePurchaseTransaction(const Utility::Purchase::PurchaseTransaction &transaction,
                                           QObject *receiver);
        QueryResult execute() override;
    private:
        QueryResult removePurchaseTransaction();
        QueryResult undoRemovePurchaseTransaction();

        void archivePurchaseTransaction();
        void archiveDebtTransaction();
        void archiveCreditTransaction();
        void revertProductQuantityUpdate();
    };
}
}

#endif // REMOVEPURCHASETRANSACTION_H
