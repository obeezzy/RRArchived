#include "viewsaletransactionitems.h"

using namespace SaleQuery;

ViewSaleTransactionItems::ViewSaleTransactionItems(qint64 transactionId,
                                                   QObject *receiver) :
    SaleExecutor(COMMAND, {
                        { "transaction_id", transactionId }
                 }, receiver)
{

}

QueryResult ViewSaleTransactionItems::execute()
{
    QueryResult result{ request() };

    return result;
}
