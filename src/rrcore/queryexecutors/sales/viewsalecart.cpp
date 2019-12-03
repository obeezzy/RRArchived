#include "viewsalecart.h"

using namespace SaleQuery;

ViewSaleCart::ViewSaleCart(qint64 transactionId, QObject *receiver) :
    SaleExecutor(COMMAND, {
                    { "transaction_id", transactionId }
                 }, receiver)
{

}

QueryResult ViewSaleCart::execute()
{
    QueryResult result{ request() };

    return result;
}
