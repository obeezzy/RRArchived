#include "dashboardsqlmanager.h"
#include <QSqlQuery>

DashboardSqlManager::DashboardSqlManager(const QString &connectionName) :
    AbstractSqlManager(connectionName)
{

}

QueryResult DashboardSqlManager::execute(const QueryRequest &request)
{
    Q_UNUSED(request);

    return QueryResult();
}
