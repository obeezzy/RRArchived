#include "addincometransaction.h"

using namespace IncomeQuery;

AddIncomeTransaction::AddIncomeTransaction(const QString &clientName,
                                           const QString &purpose,
                                           qreal amount,
                                           const QString &paymentMethod,
                                           QObject *receiver) :
    IncomeExecutor(COMMAND, {
                        { "client_name", clientName },
                        { "purpose", purpose },
                        { "amount", amount },
                        { "payment_method", paymentMethod }
                   }, receiver)
{

}

QueryResult IncomeQuery::AddIncomeTransaction::execute()
{
    QueryResult result{ request() };

    return result;
}
