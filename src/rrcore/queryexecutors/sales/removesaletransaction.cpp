#include "removesaletransaction.h"
#include "database/databaseexception.h"
#include "database/databaseerror.h"
#include "database/exceptions/exceptions.h"
#include "utility/saleutils.h"

using namespace SaleQuery;

RemoveSaleTransaction::RemoveSaleTransaction(const SaleTransaction &transaction,
                                             QObject *receiver) :
    SaleExecutor(COMMAND, {
                        { "can_undo", true },
                        { "transaction_id", transaction.id }
                 }, receiver)
{

}

QueryResult RemoveSaleTransaction::execute()
{
    throw MissingImplementationException();
}
