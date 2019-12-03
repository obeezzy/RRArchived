#include "viewsalehome.h"

using namespace SaleQuery;

ViewSaleHome::ViewSaleHome(QObject *receiver) :
    SaleExecutor(COMMAND, { }, receiver)
{

}

QueryResult ViewSaleHome::execute()
{
    QueryResult result{ request() };

    return result;
}
