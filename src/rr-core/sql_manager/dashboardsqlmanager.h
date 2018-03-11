#ifndef DASHBOARDSQLMANAGER_H
#define DASHBOARDSQLMANAGER_H

#include "abstractsqlmanager.h"

class DashboardSqlManager : public AbstractSqlManager
{
public:
    DashboardSqlManager(QSqlDatabase connection);

    QueryResult execute(const QueryRequest &request) override final;
};

#endif // DASHBOARDSQLMANAGER_H
