#include "removesoldproduct.h"
#include "database/databaseexception.h"
#include "database/databaseerror.h"
#include "database/exceptions/exceptions.h"
#include "utility/saleutils.h"

using namespace SaleQuery;

RemoveSoldProduct::RemoveSoldProduct(const Utility::SoldProduct &product,
                                     QObject *receiver) :
    SaleExecutor(COMMAND, {
                        { "sale_transaction_id", product.transaction.id },
                        { "sold_product_id", product.id }
                 }, receiver)
{

}

QueryResult RemoveSoldProduct::execute()
{
    throw MissingImplementationException();
}
