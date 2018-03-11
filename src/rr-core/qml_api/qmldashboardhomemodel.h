#ifndef QMLDASHBOARDHOMEMODEL_H
#define QMLDASHBOARDHOMEMODEL_H

#include <QObject>
#include <QAbstractListModel>

class QueryRequest;
class QueryResult;

class QMLDashboardHomeModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit QMLDashboardHomeModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override final;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override final;
signals:
    void sendRequest(const QueryRequest &);
private:
    void startQuery();
    void processResult(const QueryResult &);
};

#endif // QMLDASHBOARDHOMEMODEL_H
