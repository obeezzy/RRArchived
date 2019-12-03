#include "removepurchasetransaction.h"

using namespace PurchaseQuery;

RemovePurchaseTransaction::RemovePurchaseTransaction(qint64 transactionId,
                                                     int row,
                                                     const PurchaseTransaction &transaction,
                                                     QObject *receiver) :
    PurchaseExecutor(COMMAND, {
                        { "can_undo", true },
                            { "transaction_id", transactionId },
                            { "row", row },
                            { "record", transaction.toVariantMap() }
                     }, receiver)
{

}
