#include "qmlstockreportmodel.h"
#include "database/databasethread.h"
#include "queryexecutors/stock.h"

QMLStockReportModel::QMLStockReportModel(QObject *parent) :
    QMLStockReportModel(DatabaseThread::instance(), parent)
{}

QMLStockReportModel::QMLStockReportModel(DatabaseThread &thread, QObject *parent) :
    AbstractVisualTableModel (thread, parent)
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

    return ColumnCount;
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

QVariant QMLStockReportModel::headerData(int section, Qt::Orientation orientation, int role) const
{
   if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            switch (section) {
            case CategoryColumn:
                return tr("Category");
            case ItemColumn:
                return tr("Item");
            case OpeningStockQuantityColumn:
                return tr("Opening stock qty");
            case QuantitySoldColumn:
                return tr("Qty sold");
            case QuantityBoughtColumn:
                return tr("Qty bought");
            case QuantityInStockColumn:
                return tr("Qty in stock");
            }
        } else if (role == Qt::TextAlignmentRole) {
            switch (section) {
            case CategoryColumn:
            case ItemColumn:
                return Qt::AlignLeft;
            case OpeningStockQuantityColumn:
            case QuantitySoldColumn:
            case QuantityBoughtColumn:
            case QuantityInStockColumn:
                return Qt::AlignRight;
            }
        } else if (role == Qt::SizeHintRole) {
            switch (section) {
            case CategoryColumn:
                return 130;
            case ItemColumn:
                return tableViewWidth() - 130 -130 -130 - 130 - 130;
            case OpeningStockQuantityColumn:
                return 130;
            case QuantitySoldColumn:
                return 130;
            case QuantityBoughtColumn:
                return 130;
            case QuantityInStockColumn:
                return 130;
            }
        }
    }

    return section + 1;
}

void QMLStockReportModel::tryQuery()
{
    setBusy(true);
    emit execute(new StockQuery::ViewStockReport(this));
}

void QMLStockReportModel::processResult(const QueryResult result)
{
    if (this != result.request().receiver())
        return;

    setBusy(false);
    if (result.isSuccessful()) {
        if (result.request().command() == StockQuery::ViewStockReport::COMMAND) {
            beginResetModel();
            m_records = result.outcome().toMap().value("items").toList();
            endResetModel();
            emit success(ViewStockReportSuccess);
        }
    } else {
        emit error();
    }
}
