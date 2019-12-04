#include "qmlpurchasereportmodel.h"
#include "database/databasethread.h"
#include "queryexecutors/purchase.h"

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

    return ColumnCount;
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

QVariant QMLPurchaseReportModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            switch (section) {
            case CategoryColumn:
                return tr("Category");
            case ItemColumn:
                return tr("Item");
            case QuantityBoughtColumn:
                return tr("Qty bought");
            case TotalAmountColumn:
                return tr("Total amount");
            }
        } else if (role == Qt::TextAlignmentRole) {
            switch (section) {
            case CategoryColumn:
            case ItemColumn:
                return Qt::AlignLeft;
            case QuantityBoughtColumn:
            case TotalAmountColumn:
                return Qt::AlignRight;
            }
        } else if (role == Qt::SizeHintRole) {
            switch (section) {
            case CategoryColumn:
                return 180;
            case ItemColumn:
                return tableViewWidth() - 180 - 180 - 180;
            case QuantityBoughtColumn:
                return 180;
            case TotalAmountColumn:
                return 180;
            }
        }
    }

    return section + 1;
}

void QMLPurchaseReportModel::tryQuery()
{
    setBusy(true);
    emit execute(new PurchaseQuery::ViewPurchaseReport(this));
}

void QMLPurchaseReportModel::processResult(const QueryResult result)
{
    if (this != result.request().receiver())
        return;

    setBusy(false);
    if (result.isSuccessful()) {
        if (result.request().command() == PurchaseQuery::ViewPurchaseReport::COMMAND) {
            beginResetModel();
            m_records = result.outcome().toMap().value("items").toList();
            endResetModel();
            emit success(ViewPurchaseReportSuccess);
        } else {
            emit error();
        }
    } else {

    }
}
