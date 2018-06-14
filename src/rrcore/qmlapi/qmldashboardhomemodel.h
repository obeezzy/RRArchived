#ifndef QMLDASHBOARDHOMEMODEL_H
#define QMLDASHBOARDHOMEMODEL_H

#include <QObject>
#include <QAbstractListModel>

#include "database/queryrequest.h"
#include "database/queryresult.h"

class QMLDashboardHomeModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit QMLDashboardHomeModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override final;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override final;
signals:
    void sendRequest(const QueryRequest request);
private:
    void startQuery();
    void processResult(const QueryResult result);
};

#endif // QMLDASHBOARDHOMEMODEL_H
