#include "viewpurchasereport.h"

using namespace PurchaseQuery;

ViewPurchaseReport::ViewPurchaseReport(QObject *receiver) :
    PurchaseExecutor(COMMAND, {}, receiver)
{

}

QueryResult ViewPurchaseReport::execute()
{
    QueryResult result{ request() };

    return result;
}
