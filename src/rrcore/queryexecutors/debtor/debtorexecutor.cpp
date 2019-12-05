#include "debtorexecutor.h"

DebtorExecutor::DebtorExecutor(const QString &command,
                               const QVariantMap &params,
                               QObject *receiver) :
    QueryExecutor(command, params, QueryRequest::QueryGroup::Debtor, receiver)
{

}
