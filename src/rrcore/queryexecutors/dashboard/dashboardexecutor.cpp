#include "dashboardexecutor.h"

using namespace DashboardQuery;

DashboardExecutor::DashboardExecutor(const QString &command,
                                     const QVariantMap &params,
                                     QObject *receiver) :
    QueryExecutor(command, params, QueryRequest::QueryGroup::Dashboard, receiver)
{

}
