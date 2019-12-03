#include "saleexecutor.h"

using namespace SaleQuery;

SaleExecutor::SaleExecutor(const QString &command,
                           const QVariantMap &params,
                           QObject *receiver) :
    QueryExecutor(command, params, QueryRequest::Sales, receiver)
{

}
