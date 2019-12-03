#ifndef DASHBOARDEXECUTOR_H
#define DASHBOARDEXECUTOR_H

#include "database/queryexecutor.h"

namespace DashboardQuery {
class DashboardExecutor : public QueryExecutor
{
    Q_OBJECT
public:
    explicit DashboardExecutor(const QString &command,
                               const QVariantMap &params,
                               QObject *receiver);
    virtual ~DashboardExecutor() = default;
};
}

#endif // DASHBOARDEXECUTOR_H
