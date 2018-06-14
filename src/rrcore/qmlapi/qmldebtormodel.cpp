#include "qmldebtormodel.h"
#include "database/queryrequest.h"
#include "database/queryresult.h"
#include <QDateTime>

QMLDebtorModel::QMLDebtorModel(QObject *parent) :
    AbstractVisualListModel(parent)
{

}

int QMLDebtorModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_records.count();
}

QVariant QMLDebtorModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case ClientIdRole:
        return m_records.at(index.row()).toMap().value("client_id").toInt();
        break;
    case PreferredNameRole:
        return m_records.at(index.row()).toMap().value("preferred_name").toString();
        break;
    case TotalDebtRole:
        return m_records.at(index.row()).toMap().value("total_debt").toDouble();
        break;
    case NoteRole:
        return m_records.at(index.row()).toMap().value("note").toString();
        break;
    case CreatedRole:
        return m_records.at(index.row()).toMap().value("created").toDateTime();
        break;
    case LastEditedRole:
        return m_records.at(index.row()).toMap().value("last_edited").toDateTime();
        break;
    case UserRole:
        return m_records.at(index.row()).toMap().value("user").toString();
        break;
    }

    return QVariant();
}

QHash<int, QByteArray> QMLDebtorModel::roleNames() const
{
    QHash<int, QByteArray> roles(AbstractVisualListModel::roleNames());
    roles.insert(ClientIdRole, "client_id");
    roles.insert(PreferredNameRole, "preferred_name");
    roles.insert(TotalDebtRole, "total_debt");
    roles.insert(NoteRole, "note");
    roles.insert(CreatedRole, "created");
    roles.insert(LastEditedRole, "last_edited");
    roles.insert(UserRole, "user");

    return roles;
}

void QMLDebtorModel::tryQuery()
{
    setBusy(true);

    QueryRequest request(this);
    request.setCommand("view_debtors", QVariantMap(), QueryRequest::Debtor);

    emit executeRequest(request);
}

void QMLDebtorModel::processResult(const QueryResult result)
{
    if (result.request().receiver() != this)
        return;

    setBusy(false);

    if (result.isSuccessful()) {
        beginResetModel();
        m_records = result.outcome().toMap().value("debtors").toList();
        endResetModel();
        emit success();
    } else {
        emit error();
    }
}

void QMLDebtorModel::filter()
{
    if (filterColumn() == -1)
        return;

    QueryRequest request(this);
    QVariantMap params;

    params.insert("filter_text", filterText());

    switch (filterColumn()) {
    case PreferredNameColumn:
        params.insert("filter_column", "preferred_name");
        break;
    default:
        params.insert("filter_column", "total_debt");
        break;
    }

    request.setCommand("view_stock_items", params, QueryRequest::Stock);
    emit executeRequest(request);
}

void QMLDebtorModel::refresh()
{
    tryQuery();
}

void QMLDebtorModel::removeDebtor(int debtorId)
{
    if (debtorId <= 0)
        return;

    setBusy(true);

    QueryRequest request(this);
    request.setCommand("remove_debtor", { { "debtor_id", debtorId }, { "can_undo", true } }, QueryRequest::Debtor);
    emit executeRequest(request);
}

void QMLDebtorModel::removeItemFromModel(int debtorId)
{
    if (debtorId <= 0)
        return;

    for (int i = 0; i < m_records.count(); ++i) {
        const QVariantMap record = m_records.at(i).toMap();

        if (record.value("debtor_id").toInt() == debtorId) {
            beginRemoveRows(QModelIndex(), i, i);
            m_records.removeAt(i);
            endRemoveRows();
        }
    }
}

void QMLDebtorModel::undoRemoveItemFromModel(int debtorId, const QVariantMap &debtorInfo)
{
    if (debtorId <= 0)
        return;

    int i = 0;
    beginInsertRows(QModelIndex(), i, i);
    m_records.insert(i, debtorInfo);
    endInsertRows();
}

