#include "qmlsalereportmodel.h"
#include "database/databasethread.h"
#include "queryexecutors/sales.h"

QMLSaleReportModel::QMLSaleReportModel(QObject *parent) :
    QMLSaleReportModel(DatabaseThread::instance(), parent)
{}

QMLSaleReportModel::QMLSaleReportModel(DatabaseThread &thread, QObject *parent) :
    AbstractVisualTableModel(thread, parent)
{

}

int QMLSaleReportModel::rowCount(const QModelIndex &index) const
{
    if (index.isValid())
        return 0;

    return m_records.count();
}

int QMLSaleReportModel::columnCount(const QModelIndex &index) const
{
    if (index.isValid())
        return 0;

    return ColumnCount;
}

QVariant QMLSaleReportModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case CategoryRole:
        return m_records.at(index.row()).toMap().value("category").toString();
    case ItemRole:
        return m_records.at(index.row()).toMap().value("item").toString();
    case QuantitySoldRole:
        return m_records.at(index.row()).toMap().value("quantity_sold").toDouble();
    case UnitRole:
        return m_records.at(index.row()).toMap().value("unit").toString();
    case TotalAmountRole:
        return m_records.at(index.row()).toMap().value("total_amount").toDouble();
    }

    return QVariant();
}

QHash<int, QByteArray> QMLSaleReportModel::roleNames() const
{
    return {
        { CategoryRole, "category" },
        { ItemRole, "item" },
        { QuantitySoldRole, "quantity_sold" },
        { UnitRole, "unit" },
        { TotalAmountRole, "total_amount" }
    };
}

QVariant QMLSaleReportModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            switch (section) {
            case CategoryColumn:
                return tr("Category");
            case ItemColumn:
                return tr("Item");
            case QuantitySoldColumn:
                return tr("Qty sold");
            case TotalAmountColumn:
                return tr("Total amount");
            }
        } else if (role == Qt::TextAlignmentRole) {
            switch (section) {
            case CategoryColumn:
            case ItemColumn:
                return Qt::AlignLeft;
            case QuantitySoldColumn:
            case TotalAmountColumn:
                return Qt::AlignRight;
            }
        } else if (role == Qt::SizeHintRole) {
            switch (section) {
            case CategoryColumn:
                return 180;
            case ItemColumn:
                return tableViewWidth() - 180 - 180 - 180;
            case QuantitySoldColumn:
                return 180;
            case TotalAmountColumn:
                return 180;
            }
        }
    }

    return section + 1;
}

void QMLSaleReportModel::tryQuery()
{
    setBusy(true);
    emit execute(new SaleQuery::ViewSaleReport(this));
}

void QMLSaleReportModel::processResult(const QueryResult result)
{
    if (this != result.request().receiver())
        return;

    setBusy(false);
    if (result.isSuccessful()) {
        if (result.request().command() == SaleQuery::ViewSaleReport::COMMAND) {
            beginResetModel();
            m_records = result.outcome().toMap().value("items").toList();
            endResetModel();
            emit success(ViewSalesReportSuccess);
        } else {
            emit error();
        }
    } else {

    }
}

