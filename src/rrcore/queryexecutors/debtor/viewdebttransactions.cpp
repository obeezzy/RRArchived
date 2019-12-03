#include "viewdebttransactions.h"

using namespace DebtorQuery;

ViewDebtTransactions::ViewDebtTransactions(int debtorId,
                                           QObject *receiver) :
    DebtorExecutor(COMMAND, { { "debtor_id", debtorId } }, receiver)
{

}

QueryResult ViewDebtTransactions::execute()
{
    QueryResult result{ request() };

    return result;
}
