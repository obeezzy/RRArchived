#include "debtorexecutor.h"

DebtorExecutor::DebtorExecutor(const QString &command,
                               const QVariantMap &params,
                               QObject *receiver) :
    QueryExecutor(command, params, QueryRequest::Debtor, receiver)
{

}
