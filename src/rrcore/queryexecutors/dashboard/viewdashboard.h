#ifndef VIEWDASHBOARD_H
#define VIEWDASHBOARD_H

#include "dashboardexecutor.h"

namespace DashboardQuery {
class ViewDashboard : public DashboardExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_dashboard");

    explicit ViewDashboard(QObject *receiver);
    QueryResult execute() override;
private:
    void fetchWelcomeMessage(QueryResult &result);
    void fetchEmptyStockMessage(QueryResult &result);
};
}

#endif // VIEWDASHBOARD_H
