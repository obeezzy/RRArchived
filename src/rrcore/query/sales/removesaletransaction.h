#ifndef REMOVESALETRANSACTION_H
#define REMOVESALETRANSACTION_H

#include "saleexecutor.h"

namespace Utility {
namespace Sales {
class SaleTransaction;
}
}

namespace Query {
    namespace Sales {
    class RemoveSaleTransaction : public SaleExecutor
    {
        Q_OBJECT
    public:
        static inline const QString COMMAND = QStringLiteral("remove_sale_transaction");
        static inline const QString UNDO_COMMAND = QStringLiteral("undo_remove_sale_transaction");

        explicit RemoveSaleTransaction(const Utility::Sales::SaleTransaction &transaction,
                                       QObject *receiver);
        QueryResult execute() override;
    };
}
}

#endif // REMOVESALETRANSACTION_H
