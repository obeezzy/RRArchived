#include "viewpurchasehome.h"

using namespace PurchaseQuery;

ViewPurchaseHome::ViewPurchaseHome(QObject *receiver) :
    PurchaseExecutor(COMMAND, { }, receiver)
{

}

QueryResult ViewPurchaseHome::execute()
{
    QueryResult result{ request() };

    return result;
}
