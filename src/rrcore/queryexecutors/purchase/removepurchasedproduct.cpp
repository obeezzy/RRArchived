#include "removepurchasedproduct.h"

using namespace PurchaseQuery;

RemovePurchasedProduct::RemovePurchasedProduct(qint64 transactionId,
                                               int purchasedProductId,
                                               QObject *receiver) :
    PurchaseExecutor(COMMAND, {
                        { "can_undo", true },
                        { "transaction_id", transactionId },
                        { "purchased_product_id", purchasedProductId }
                     }, receiver)
{

}

QueryResult RemovePurchasedProduct::execute()
{
    QueryResult result{ request() };

    return result;
}
