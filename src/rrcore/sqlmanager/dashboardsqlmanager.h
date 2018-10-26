#ifndef DASHBOARDSQLMANAGER_H
#define DASHBOARDSQLMANAGER_H

#include "abstractsqlmanager.h"

class DashboardSqlManager : public AbstractSqlManager
{
public:
    DashboardSqlManager(const QString &connectionName);

    QueryResult execute(const QueryRequest &request) override final;
};

#endif // DASHBOARDSQLMANAGER_H
