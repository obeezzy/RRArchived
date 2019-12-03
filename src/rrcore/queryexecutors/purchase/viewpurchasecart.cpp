#include "viewpurchasecart.h"

using namespace PurchaseQuery;

ViewPurchaseCart::ViewPurchaseCart(qint64 transactionId,
                                   QObject *receiver) :
    PurchaseExecutor(COMMAND, {
                        { "transaction_id", transactionId }
                     }, receiver)
{

}

QueryResult ViewPurchaseCart::execute()
{
    QueryResult result{ request() };

    return result;
}
