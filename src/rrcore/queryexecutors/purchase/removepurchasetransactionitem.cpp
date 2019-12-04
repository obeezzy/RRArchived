#include "removepurchasetransactionitem.h"

using namespace PurchaseQuery;

RemovePurchaseTransactionItem::RemovePurchaseTransactionItem(qint64 transactionId,
                                                             int transactionItemId,
                                                             QObject *receiver) :
    PurchaseExecutor(COMMAND, {
                        { "can_undo", true },
                        { "transaction_id", transactionId },
                        { "transaction_item_id", transactionItemId }
                     }, receiver)
{

}

QueryResult RemovePurchaseTransactionItem::execute()
{
    QueryResult result{ request() };

    return result;
}
