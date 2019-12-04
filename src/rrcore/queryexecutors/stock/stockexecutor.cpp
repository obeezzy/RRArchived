#include "stockexecutor.h"

StockExecutor::StockExecutor(const QString &command,
                             const QVariantMap &params,
                             QObject *receiver) :
    QueryExecutor(command, params, QueryRequest::Stock, receiver)
{

}
