#include "addexpensetransaction.h"

using namespace ExpenseQuery;

AddExpenseTransaction::AddExpenseTransaction(const QString &clientName,
                                             const QString &purpose,
                                             qreal amount,
                                             const QString &paymentMethod,
                                             QObject *receiver) :
    ExpenseExecutor(COMMAND, {
                        { "client_name", clientName },
                        { "purpose", purpose },
                        { "amount", amount },
                        { "payment_method", paymentMethod }
                    }, receiver)
{

}

QueryResult AddExpenseTransaction::execute()
{
    QueryResult result{ request() };

    return result;
}
