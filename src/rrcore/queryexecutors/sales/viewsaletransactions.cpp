#include "viewsaletransactions.h"

using namespace SaleQuery;

ViewSaleTransactions::ViewSaleTransactions(const QDateTime &from,
                                           const QDateTime &to,
                                           bool suspended,
                                           bool archived,
                                           QObject *receiver) :
    SaleExecutor(COMMAND, {
                        { "from", from },
                        { "to", to },
                        { "suspended", suspended },
                        { "archived", archived }
                 }, receiver)
{

}

QueryResult ViewSaleTransactions::execute()
{
    QueryResult result{ request() };

    return result;
}
