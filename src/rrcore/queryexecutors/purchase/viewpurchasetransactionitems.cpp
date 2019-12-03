#include "viewpurchasetransactionitems.h"

using namespace PurchaseQuery;

ViewPurchaseTransactionItems::ViewPurchaseTransactionItems(qint64 transactionId,
                                                           QObject *receiver) :
    PurchaseExecutor(COMMAND, {
                        { "transaction_id", transactionId }
                     }, receiver)
{

}

QueryResult ViewPurchaseTransactionItems::execute()
{
    QueryResult result{ request() };

    return result;
}
