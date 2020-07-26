#include "qmlmostsoldproductmodel.h"
#include "database/databasethread.h"
#include "query/sales/viewmostsoldproducts.h"
#include <QDebug>

QMLMostSoldProductModel::QMLMostSoldProductModel(QObject *parent) :
    QMLMostSoldProductModel(DatabaseThread::instance(), parent)
{}

QMLMostSoldProductModel::QMLMostSoldProductModel(DatabaseThread &thread,
                                                 QObject *parent) :
    AbstractKpiModel(thread, parent)
{}

int QMLMostSoldProductModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_products.count();
}

int QMLMostSoldProductModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return ColumnCount;
}

QVariant QMLMostSoldProductModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
        switch (index.column()) {
        case ProductColumn:
            return m_products.at(index.row()).toMap().value("product").toString();
        case TotalQuantityColumn:
            return m_products.at(index.row()).toMap().value("total_quantity").toDouble();
        case TotalRevenueColumn:
            return m_products.at(index.row()).toMap().value("total_revenue").toDouble();
        default:
            return QVariant();
        }
    case ProductCategoryIdRole:
        return m_products.at(index.row()).toMap().value("product_category_id").toInt();
    case ProductCategoryRole:
        return m_products.at(index.row()).toMap().value("product_category").toString();
    case ProductIdRole:
        return m_products.at(index.row()).toMap().value("product_id").toInt();
    case ProductRole:
        return m_products.at(index.row()).toMap().value("product").toString();
    case TotalRevenueRole:
        return m_products.at(index.row()).toMap().value("total_revenue").toDouble();
    case TotalQuantityRole:
        return m_products.at(index.row()).toMap().value("total_quantity").toDouble();
    case ProductUnitIdRole:
        return m_products.at(index.row()).toMap().value("product_unit_id").toInt();
    case ProductUnitRole:
        return m_products.at(index.row()).toMap().value("product_unit").toString();
    }

    return QVariant();
}

QHash<int, QByteArray> QMLMostSoldProductModel::roleNames() const
{
    return {
        { Qt::DisplayRole, "display" },
        { ProductCategoryIdRole, "product_category_id" },
        { ProductCategoryRole, "product_category" },
        { ProductIdRole, "product_id" },
        { ProductRole, "product" },
        { TotalRevenueRole, "total_revenue" },
        { TotalQuantityRole, "total_quantity" },
        { ProductUnitIdRole, "product_unit_id" },
        { ProductUnitRole, "product_unit" }
    };
}

void QMLMostSoldProductModel::tryQuery()
{
    setBusy(true);
    emit execute(new Query::Sales::ViewMostSoldProducts(dateTimeSpan(),
                                                         this));
}

bool QMLMostSoldProductModel::canProcessResult(const QueryResult &result) const
{
    Q_UNUSED(result)
    return true;
}

void QMLMostSoldProductModel::processResult(const QueryResult &result)
{
    setBusy(false);

    if (result.isSuccessful()) {
        beginResetModel();
        m_products = result.outcome().toMap().value("products").toList();
        endResetModel();
        emit success();
    } else {
        emit error();
    }
}
