#include "saletotalrevenuemodel.h"
#include <QDate>
#include <QDebug>

SaleTotalRevenueModel::SaleTotalRevenueModel(const QVariantList &records, QObject *parent) :
    AbstractVisualListModel(parent),
    m_records(records)
{
}

int SaleTotalRevenueModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_records.count();
}

int SaleTotalRevenueModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 2;
}

QVariant SaleTotalRevenueModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
        switch (index.column()) {
        case 0:
            return m_records.at(index.row()).toMap().value("created").toDate();
        case 1:
            return m_records.at(index.row()).toMap().value("amount_paid").toDouble();
        }
    case CreatedRole:
        return m_records.at(index.row()).toMap().value("created").toDate();
    case AmountPaidRole:
        return m_records.at(index.row()).toMap().value("amount_paid").toDouble();
    }

    return QVariant();
}

QHash<int, QByteArray> SaleTotalRevenueModel::roleNames() const
{
    QHash<int, QByteArray> roles(AbstractVisualListModel::roleNames());
    roles.insert(CreatedRole, "created");
    roles.insert(AmountPaidRole, "amount_paid");

    return roles;
}

void SaleTotalRevenueModel::tryQuery()
{

}

void SaleTotalRevenueModel::processResult(const QueryResult &result)
{
    Q_UNUSED(result)
}
