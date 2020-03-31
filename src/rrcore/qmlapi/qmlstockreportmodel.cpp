#include "qmlstockreportmodel.h"
#include "database/databasethread.h"
#include "queryexecutors/stock.h"

QMLStockReportModel::QMLStockReportModel(QObject *parent) :
    QMLStockReportModel(DatabaseThread::instance(), parent)
{}

QMLStockReportModel::QMLStockReportModel(DatabaseThread &thread,
                                         QObject *parent) :
    AbstractReportModel(thread, parent)
{
}

int QMLStockReportModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_transactions.count();
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
        return m_transactions.at(index.row()).category.category;
    case ProductRole:
        return m_transactions.at(index.row()).product.product;
    case OpeningStockQuantityRole:
        return m_transactions.at(index.row()).openingStockQuantity.toDouble();
    case QuantitySoldRole:
        return m_transactions.at(index.row()).quantitySold.toDouble();
    case QuantityBoughtRole:
        return m_transactions.at(index.row()).quantityBought.toDouble();
    case QuantityInStockRole:
        return m_transactions.at(index.row()).quantityInStock.toDouble();
    case UnitRole:
        return m_transactions.at(index.row()).product.unit.unit;
    }

    return QVariant();
}

QHash<int, QByteArray> QMLStockReportModel::roleNames() const
{
    return {
        { CategoryRole, "category" },
        { ProductRole, "product" },
        { OpeningStockQuantityRole, "opening_stock_quantity" },
        { QuantitySoldRole, "quantity_sold" },
        { QuantityBoughtRole, "quantity_bought" },
        { QuantityInStockRole, "quantity_in_stock" },
        { UnitRole, "unit" }
    };
}

QVariant QMLStockReportModel::headerData(int section,
                                         Qt::Orientation orientation,
                                         int role) const
{
   if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            switch (section) {
            case CategoryColumn:
                return tr("Category");
            case ProductColumn:
                return tr("Product");
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
            case ProductColumn:
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
            case ProductColumn:
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
    emit execute(new StockQuery::ViewStockReport(dateTimeSpan(),
                                                 this));
}

bool QMLStockReportModel::canProcessResult(const QueryResult &result) const
{
    Q_UNUSED(result)
    return true;
}

void QMLStockReportModel::processResult(const QueryResult &result)
{
    setBusy(false);
    if (result.isSuccessful()) {
        if (result.request().command() == StockQuery::ViewStockReport::COMMAND) {
            beginResetModel();
            m_transactions = Utility::StockReportTransactionList{ result.outcome().toMap().value("transactions").toList() };
            endResetModel();
            emit success();
        }
    } else {
        emit error();
    }
}
