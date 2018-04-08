#include "salemostsolditemmodel.h"
#include <QDebug>

SaleMostSoldItemModel::SaleMostSoldItemModel(const QVariantList &records, QObject *parent) :
    AbstractVisualListModel(parent),
    m_records(records)
{

}

int SaleMostSoldItemModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_records.count();
}

int SaleMostSoldItemModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 2;
}

QVariant SaleMostSoldItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
        switch (index.column()) {
        case 0:
            return m_records.at(index.row()).toMap().value("item").toString();
        case 1:
            return m_records.at(index.row()).toMap().value("total_quantity").toDouble();
        }
    case CategoryIdRole:
        return m_records.at(index.row()).toMap().value("category_id").toInt();
    case CategoryRole:
        return m_records.at(index.row()).toMap().value("category").toInt();
    case ItemIdRole:
        return m_records.at(index.row()).toMap().value("item_id").toInt();
    case ItemRole:
        return m_records.at(index.row()).toMap().value("item").toString();
    case TotalRevenueRole:
        return m_records.at(index.row()).toMap().value("total_revenue").toDouble();
    case TotalQuantityRole:
        return m_records.at(index.row()).toMap().value("total_quantity").toDouble();
    case UnitIdRole:
        return m_records.at(index.row()).toMap().value("unit_id").toInt();
    case UnitRole:
        return m_records.at(index.row()).toMap().value("unit").toString();
    }

    return QVariant();
}

QHash<int, QByteArray> SaleMostSoldItemModel::roleNames() const
{
    QHash<int, QByteArray> roles(AbstractVisualListModel::roleNames());
    roles.insert(CategoryIdRole, "category_id");
    roles.insert(CategoryRole, "category");
    roles.insert(ItemIdRole, "item_id");
    roles.insert(ItemRole, "item");
    roles.insert(TotalRevenueRole, "total_revenue");
    roles.insert(TotalQuantityRole, "total_quantity");
    roles.insert(UnitIdRole, "unit_id");
    roles.insert(UnitRole, "unit");

    return roles;
}

void SaleMostSoldItemModel::tryQuery()
{

}

void SaleMostSoldItemModel::processResult(const QueryResult &result)
{
    Q_UNUSED(result)
}
