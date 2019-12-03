#include "removesaletransaction.h"

using namespace SaleQuery;

RemoveSaleTransaction::RemoveSaleTransaction(qint64 transactionId,
                                             QObject *receiver) :
    SaleExecutor(COMMAND, {
                        { "can_undo", true },
                        { "transaction_id", transactionId }
                 }, receiver)
{

}

QueryResult RemoveSaleTransaction::execute()
{
    QueryResult result{ request() };

    return result;
}
