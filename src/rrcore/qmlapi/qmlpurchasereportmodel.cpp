#include "qmlpurchasereportmodel.h"
#include "database/databasethread.h"

const int COLUMN_COUNT = 5;

QMLPurchaseReportModel::QMLPurchaseReportModel(QObject *parent) :
    QMLPurchaseReportModel(DatabaseThread::instance(), parent)
{}

QMLPurchaseReportModel::QMLPurchaseReportModel(DatabaseThread &thread, QObject *parent) :
    AbstractVisualTableModel(thread, parent)
{

}

QVariant QMLPurchaseReportModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case CategoryRole:
        return m_records.at(index.row()).toMap().value("category").toString();
    case ItemRole:
        return m_records.at(index.row()).toMap().value("item").toString();
    case QuantityBoughtRole:
        return m_records.at(index.row()).toMap().value("quantity_bought").toDouble();
    case UnitRole:
        return m_records.at(index.row()).toMap().value("unit").toString();
    case TotalAmountRole:
        return m_records.at(index.row()).toMap().value("total_amount").toDouble();
    }

    return QVariant();
}

int QMLPurchaseReportModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_records.count();
}

int QMLPurchaseReportModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return COLUMN_COUNT;
}

QHash<int, QByteArray> QMLPurchaseReportModel::roleNames() const
{
    return {
        { CategoryRole, "category" },
        { ItemRole, "item" },
        { QuantityBoughtRole, "quantity_bought" },
        { TotalAmountRole, "total_amount" },
        { UnitRole, "unit" }
    };
}

void QMLPurchaseReportModel::tryQuery()
{
    setBusy(true);
    QueryRequest request(this);
    request.setCommand("view_purchase_report", { }, QueryRequest::Purchase);
    emit executeRequest(request);
}

void QMLPurchaseReportModel::processResult(const QueryResult result)
{
    if (this != result.request().receiver())
        return;

    setBusy(false);
    if (result.isSuccessful()) {
        if (result.request().command() == "view_purchase_report") {
            beginResetModel();
            m_records = result.outcome().toMap().value("items").toList();
            endResetModel();
            emit success(ViewPurchaseReportSuccess);
        } else {
            emit success();
        }
    } else {

    }
}
