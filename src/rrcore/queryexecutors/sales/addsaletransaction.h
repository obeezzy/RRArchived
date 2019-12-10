#ifndef ADDSALETRANSACTION_H
#define ADDSALETRANSACTION_H

#include "saleexecutor.h"
#include "utility/saleutils.h"
#include "utility/stockutils.h"

namespace SaleQuery {
class AddSaleTransaction : public SaleExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = "add_sale_transaction";
    static inline const QString UNDO_COMMAND = "undo_add_sale_transaction";

    explicit AddSaleTransaction(qint64 transactionId,
                                const QString &customerName,
                                int clientId,
                                const QString &customerPhoneNumber,
                                qreal totalCost,
                                qreal amountPaid,
                                qreal balance,
                                bool suspended,
                                const QDateTime &dueDate,
                                const QString &action,
                                const QString &note,
                                const SalePaymentList &payments,
                                const StockItemList &items,
                                QObject *receiver);
    explicit AddSaleTransaction(const QueryRequest &request, QObject *receiver);
    QueryResult execute() override;
private:
    QueryResult undoAddSaleTransaction();
};
}

#endif // ADDSALETRANSACTION_H
