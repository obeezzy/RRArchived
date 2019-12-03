#include "removedebtor.h"

using namespace DebtorQuery;

RemoveDebtor::RemoveDebtor(int debtorId,
                           int debtorRow,
                           QObject *receiver) :
    DebtorExecutor(COMMAND, {
                        { "can_undo", true },
                        { "debtor_id", debtorId },
                        { "debtor_row", debtorRow }
                   }, receiver)
{

}

QueryResult RemoveDebtor::execute()
{
    QueryResult result{ request() };

    return result;
}
