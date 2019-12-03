#include "purchaseexecutor.h"

PurchaseExecutor::PurchaseExecutor(const QString &command,
                                   const QVariantMap &params,
                                   QObject *receiver) :
    QueryExecutor(command, params, QueryRequest::Purchase, receiver)
{

}
