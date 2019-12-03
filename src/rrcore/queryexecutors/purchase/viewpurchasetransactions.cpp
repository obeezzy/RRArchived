#include "viewpurchasetransactions.h"

using namespace PurchaseQuery;

ViewPurchaseTransactions::ViewPurchaseTransactions(const QDateTime &from,
                                                   const QDateTime &to,
                                                   bool suspended,
                                                   bool archived,
                                                   QObject *receiver) :
    PurchaseExecutor(COMMAND, {
                        { "from", from },
                        { "to", to },
                        { "suspended", suspended },
                        { "archived", archived }
                     }, receiver)
{

}

QueryResult PurchaseQuery::ViewPurchaseTransactions::execute()
{
    QueryResult result{ request() };

    return result;
}
