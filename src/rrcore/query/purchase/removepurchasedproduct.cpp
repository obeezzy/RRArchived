#include "removepurchasedproduct.h"
#include "utility/purchase/purchasedproduct.h"

using namespace Query::Purchase;

RemovePurchasedProduct::RemovePurchasedProduct(const Utility::Purchase::PurchasedProduct &product,
                                               QObject *receiver) :
    PurchaseExecutor(COMMAND, {
                        { "can_undo", true },
                        { "purchase_transaction_id", product.transaction.id },
                        { "purchased_product_id", product.id }
                     }, receiver)
{}

QueryResult RemovePurchasedProduct::execute()
{
    QueryResult result{ request() };

    return result;
}
