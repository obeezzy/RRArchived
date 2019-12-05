#include "expenseexecutor.h"

ExpenseExecutor::ExpenseExecutor(const QString &command,
                                 const QVariantMap &params,
                                 QObject *receiver) :
    QueryExecutor(command, params, QueryRequest::QueryGroup::Expense, receiver)
{

}
