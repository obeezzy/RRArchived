#include "qmlsalereportmodel.h"
#include "database/databasethread.h"
#include "query/sales/viewsalereport.h"
#include "utility/common/datetimespan.h"

QMLSaleReportModel::QMLSaleReportModel(QObject *parent) :
    QMLSaleReportModel(DatabaseThread::instance(), parent)
{}

QMLSaleReportModel::QMLSaleReportModel(DatabaseThread &thread,
                                       QObject *parent) :
    AbstractReportModel(thread, parent)
{

}

int QMLSaleReportModel::rowCount(const QModelIndex &index) const
{
    if (index.isValid())
        return 0;

    return m_transactions.count();
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
        return m_transactions.at(index.row()).category.category;
    case ProductRole:
        return m_transactions.at(index.row()).product.product;
    case QuantitySoldRole:
        return m_transactions.at(index.row()).quantitySold.toDouble();
    case UnitRole:
        return m_transactions.at(index.row()).product.unit.unit;
    case TotalRevenueRole:
        return m_transactions.at(index.row()).totalRevenue.toDouble();
    }

    return QVariant();
}

QHash<int, QByteArray> QMLSaleReportModel::roleNames() const
{
    return {
        { CategoryRole, "category" },
        { ProductRole, "product" },
        { QuantitySoldRole, "quantity_sold" },
        { UnitRole, "unit" },
        { TotalRevenueRole, "total_revenue" }
    };
}

QVariant QMLSaleReportModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            switch (section) {
            case CategoryColumn:
                return tr("Category");
            case ProductColumn:
                return tr("Product");
            case QuantitySoldColumn:
                return tr("Qty sold");
            case TotalRevenueColumn:
                return tr("Total revenue");
            }
        } else if (role == Qt::TextAlignmentRole) {
            switch (section) {
            case CategoryColumn:
            case ProductColumn:
                return Qt::AlignLeft;
            case QuantitySoldColumn:
            case TotalRevenueColumn:
                return Qt::AlignRight;
            }
        } else if (role == Qt::SizeHintRole) {
            switch (section) {
            case CategoryColumn:
                return 180;
            case ProductColumn:
                return tableViewWidth() - 180 - 180 - 180;
            case QuantitySoldColumn:
                return 180;
            case TotalRevenueColumn:
                return 180;
            }
        }
    }

    return section + 1;
}

void QMLSaleReportModel::tryQuery()
{
    setBusy(true);
    emit execute(new Query::Sales::ViewSaleReport(dateTimeSpan(),
                                                   this));
}

bool QMLSaleReportModel::canProcessResult(const QueryResult &result) const
{
    Q_UNUSED(result)
    return true;
}

void QMLSaleReportModel::processResult(const QueryResult &result)
{
    setBusy(false);
    if (result.isSuccessful()) {
        if (result.request().command() == Query::Sales::ViewSaleReport::COMMAND) {
            beginResetModel();
            m_transactions = Utility::Sales::SaleReportTransactionList{ result.outcome().toMap().value("transactions").toList() };
            endResetModel();
            emit success();
        } else {
            emit error();
        }
    } else {

    }
}

