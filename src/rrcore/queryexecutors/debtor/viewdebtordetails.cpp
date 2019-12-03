#include "viewdebtordetails.h"

using namespace DebtorQuery;

ViewDebtorDetails::ViewDebtorDetails(int debtorId,
                                     QObject *receiver) :
    DebtorExecutor(COMMAND, {
                        { "debtor_id", debtorId }
                   }, receiver)
{

}

QueryResult ViewDebtorDetails::execute()
{
    QueryResult result{ request() };

    return result;
}
