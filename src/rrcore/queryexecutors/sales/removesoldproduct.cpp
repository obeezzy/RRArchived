#include "removesoldproduct.h"
#include "database/databaseexception.h"
#include "database/databaseerror.h"
#include "database/exceptions/exceptions.h"

using namespace SaleQuery;

RemoveSoldProduct::RemoveSoldProduct(qint64 transactionId,
                                                     int soldProductId,
                                                     QObject *receiver) :
    SaleExecutor(COMMAND, {
                        { "transaction_id", transactionId },
                        { "sold_product_id", soldProductId }
                 }, receiver)
{

}

QueryResult RemoveSoldProduct::execute()
{
    throw MissingImplementationException();
}
