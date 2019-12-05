#include "incomeexecutor.h"

IncomeExecutor::IncomeExecutor(const QString &command,
                               const QVariantMap &params,
                               QObject *receiver) :
    QueryExecutor(command, params, QueryRequest::QueryGroup::Income, receiver)
{

}
