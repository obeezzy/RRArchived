#include "qmltotalrevenuemodel.h"
#include <QDate>
#include <QDebug>
#include "database/databasethread.h"

QMLTotalRevenueModel::QMLTotalRevenueModel(QObject* parent)
    : QMLTotalRevenueModel(DatabaseThread::instance(), parent)
{}

QMLTotalRevenueModel::QMLTotalRevenueModel(DatabaseThread& thread,
                                           QObject* parent)
    : AbstractVisualListModel(thread, parent)
{}

int QMLTotalRevenueModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;

    return m_records.count();
}

int QMLTotalRevenueModel::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;

    return 2;
}

QVariant QMLTotalRevenueModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
        case Qt::DisplayRole:
            switch (index.column()) {
                case 0:
                    return m_records.at(index.row())
                        .toMap()
                        .value("created")
                        .toDate();
                case 1:
                    return m_records.at(index.row())
                        .toMap()
                        .value("amount_paid")
                        .toDouble();
            }
            break;
        case CreatedRole:
            return m_records.at(index.row()).toMap().value("created").toDate();
        case AmountPaidRole:
            return m_records.at(index.row())
                .toMap()
                .value("amount_paid")
                .toDouble();
    }

    return QVariant();
}

QHash<int, QByteArray> QMLTotalRevenueModel::roleNames() const
{
    return {{CreatedRole, "created"}, {AmountPaidRole, "amount_paid"}};
}

void QMLTotalRevenueModel::tryQuery() {}

bool QMLTotalRevenueModel::canProcessResult(const QueryResult& result) const
{
    Q_UNUSED(result)
    return true;
}

void QMLTotalRevenueModel::processResult(const QueryResult& result)
{
    Q_UNUSED(result)
}
