#include "qmlstockreportmodel.h"

const int COLUMN_COUNT = 7;

QMLStockReportModel::QMLStockReportModel(QObject *parent) :
    AbstractVisualTableModel (parent)
{

}

QMLStockReportModel::QMLStockReportModel(DatabaseThread &thread) :
    AbstractVisualTableModel (thread)
{

}

int QMLStockReportModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_records.count();
}

int QMLStockReportModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return COLUMN_COUNT;
}

QVariant QMLStockReportModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case CategoryRole:
        return m_records.at(index.row()).toMap().value("category").toString();
    case ItemRole:
        return m_records.at(index.row()).toMap().value("item").toString();
    case OpeningStockQuantityRole:
        return m_records.at(index.row()).toMap().value("opening_stock_quantity").toString();
    case QuantitySoldRole:
        return m_records.at(index.row()).toMap().value("quantity_sold").toString();
    case QuantityBoughtRole:
        return m_records.at(index.row()).toMap().value("quantity_bought").toString();
    case QuantityInStockRole:
        return m_records.at(index.row()).toMap().value("quantity_in_stock").toString();
    case UnitRole:
        return m_records.at(index.row()).toMap().value("unit").toString();
    }

    return QVariant();
}

QHash<int, QByteArray> QMLStockReportModel::roleNames() const
{
    return {
        { CategoryRole, "category" },
        { ItemRole, "item" },
        { OpeningStockQuantityRole, "opening_stock_quantity" },
        { QuantitySoldRole, "quantity_sold" },
        { QuantityBoughtRole, "quantity_bought" },
        { QuantityInStockRole, "quantity_in_stock" },
        { UnitRole, "unit" }
    };
}

void QMLStockReportModel::tryQuery()
{
    setBusy(true);
    QueryRequest request(this);
    request.setCommand("view_stock_report", { }, QueryRequest::Stock);
    emit executeRequest(request);
}

void QMLStockReportModel::processResult(const QueryResult result)
{
    if (this != result.request().receiver())
        return;

    setBusy(false);
    if (result.isSuccessful()) {
        if (result.request().command() == "view_stock_report") {
            beginResetModel();
            m_records = result.outcome().toMap().value("items").toList();
            endResetModel();
            emit success(ViewStockReportSuccess);
        }
    } else {
        emit error();
    }
}
