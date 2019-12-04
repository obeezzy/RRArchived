#include "removesaletransactionitem.h"
#include "database/databaseexception.h"
#include "database/databaseerror.h"

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
    throw DatabaseException(DatabaseError::QueryErrorCode::NotYetImplementedError);
}
