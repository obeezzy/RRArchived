#include "qmldashboardhomemodel.h"
#include <QTimer>
#include <QDebug>

#include "database/databasethread.h"

QMLDashboardHomeModel::QMLDashboardHomeModel(QObject *parent)
    : QAbstractListModel(parent)
{
    connect(this, &QMLDashboardHomeModel::sendRequest, &DatabaseThread::instance(), &DatabaseThread::execute);
    connect(&DatabaseThread::instance(), &DatabaseThread::resultReady, this, &QMLDashboardHomeModel::processResult);

    QTimer::singleShot(1500, Qt::CoarseTimer, this, &QMLDashboardHomeModel::startQuery);
}

QMLDashboardHomeModel::QMLDashboardHomeModel(DatabaseThread &thread)
{
    connect(this, &QMLDashboardHomeModel::sendRequest, &thread, &DatabaseThread::execute);
    connect(&thread, &DatabaseThread::resultReady, this, &QMLDashboardHomeModel::processResult);

    QTimer::singleShot(1500, Qt::CoarseTimer, this, &QMLDashboardHomeModel::startQuery);
}

int QMLDashboardHomeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 1;
}

QVariant QMLDashboardHomeModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(index)
    Q_UNUSED(role)

    return QVariant();
}

void QMLDashboardHomeModel::startQuery()
{
    QueryRequest request(this);
    request.setCommand("view_dashboard", QVariantMap(), QueryRequest::Dashboard);
    emit sendRequest(request);
}

void QMLDashboardHomeModel::processResult(const QueryResult result)
{
    if (this != result.request().receiver())
        return;

    if (result.isSuccessful()) {

    } else {

    }
}
