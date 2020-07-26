#include "removesaletransaction.h"
#include "database/databaseerror.h"
#include "database/databaseexception.h"
#include "database/exceptions/exceptions.h"
#include "utility/sales/saletransaction.h"

using namespace Query::Sales;

RemoveSaleTransaction::RemoveSaleTransaction(
    const Utility::Sales::SaleTransaction& transaction, QObject* receiver)
    : SaleExecutor(COMMAND,
                   {{"can_undo", true}, {"transaction_id", transaction.id}},
                   receiver)
{}

QueryResult RemoveSaleTransaction::execute()
{
    throw MissingImplementationException();
}
