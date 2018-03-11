#include "dashboardsqlmanager.h"
#include <QSqlQuery>

DashboardSqlManager::DashboardSqlManager(QSqlDatabase connection) :
    AbstractSqlManager(connection)
{

}

QueryResult DashboardSqlManager::execute(const QueryRequest &request)
{
    Q_UNUSED(request);

    return QueryResult();
}
