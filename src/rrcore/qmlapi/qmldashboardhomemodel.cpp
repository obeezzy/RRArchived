#include "qmldashboardhomemodel.h"
#include <QTimer>
#include <QDebug>

#include "database/databasethread.h"

QMLDashboardHomeModel::QMLDashboardHomeModel(QObject *parent)
    : AbstractHomeModel(parent)
{
}

QMLDashboardHomeModel::QMLDashboardHomeModel(DatabaseThread &thread)
    : AbstractHomeModel(thread)
{
}

void QMLDashboardHomeModel::tryQuery()
{
    setBusy(true);
    QueryRequest request(this);
    request.setCommand("view_dashboard", { }, QueryRequest::Dashboard);
    emit executeRequest(request);
}
