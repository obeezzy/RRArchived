#include "removepurchasedproduct.h"
#include "utility/purchaseutils.h"

using namespace PurchaseQuery;

RemovePurchasedProduct::RemovePurchasedProduct(const Utility::PurchasedProduct &product,
                                               QObject *receiver) :
    PurchaseExecutor(COMMAND, {
                        { "can_undo", true },
                        { "purchase_transaction_id", product.purchaseTransactionId },
                        { "purchased_product_id", product.id }
                     }, receiver)
{

}

QueryResult RemovePurchasedProduct::execute()
{
    QueryResult result{ request() };

    return result;
}
