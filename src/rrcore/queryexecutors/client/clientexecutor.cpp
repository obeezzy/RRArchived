#include "clientexecutor.h"

ClientExecutor::ClientExecutor(const QString &command,
                               const QVariantMap &params,
                               QObject *receiver) :
    QueryExecutor(command, params, QueryRequest::QueryGroup::Client, receiver)
{

}
