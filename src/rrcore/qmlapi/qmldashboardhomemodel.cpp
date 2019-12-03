#include "qmldashboardhomemodel.h"
#include <QTimer>
#include <QDebug>

#include "database/databasethread.h"
#include "queryexecutors/dashboard.h"

QMLDashboardHomeModel::QMLDashboardHomeModel(QObject *parent)
    : QMLDashboardHomeModel(DatabaseThread::instance(), parent)
{}

QMLDashboardHomeModel::QMLDashboardHomeModel(DatabaseThread &thread, QObject *parent)
    : AbstractHomeModel(thread, parent)
{
}

void QMLDashboardHomeModel::tryQuery()
{
    setBusy(true);
    emit execute(new DashboardQuery::ViewDashboard(this));
}
