#include "removesaletransactionitem.h"

using namespace SaleQuery;

RemoveSaleTransactionItem::RemoveSaleTransactionItem(qint64 transactionId,
                                                     int transactionItemId,
                                                     QObject *receiver) :
    SaleExecutor(COMMAND, {
                        { "transaction_id", transactionId },
                        { "transaction_item_id", transactionItemId }
                 }, receiver)
{

}

QueryResult RemoveSaleTransactionItem::execute()
{
    QueryResult result{ request() };

    return result;
}
