#include "removesaletransaction.h"
#include "database/databaseexception.h"
#include "database/databaseerror.h"

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
    throw DatabaseException(DatabaseError::QueryErrorCode::NotYetImplementedError);
}
