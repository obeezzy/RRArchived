#ifndef DASHBOARDSQLMANAGER_H
#define DASHBOARDSQLMANAGER_H

#include "abstractsqlmanager.h"

class DashboardSqlManager : public AbstractSqlManager
{
public:
    DashboardSqlManager(const QString &connectionName);

    QueryResult execute(const QueryRequest &request) override final;
private:
    void viewDashboard(const QueryRequest &request, QueryResult &result);

    void fetchWelcomeMessage(QueryResult &result);
    void fetchEmptyStockMessage(QueryResult &result);
};

#endif // DASHBOARDSQLMANAGER_H
