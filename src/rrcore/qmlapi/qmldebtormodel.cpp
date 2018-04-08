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
    case NameRole:
        return m_records.at(index.row()).toMap().value("name").toString();
        break;
    case TotalBalanceRole:
        return m_records.at(index.row()).toMap().value("total_balance").toDouble();
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
    roles.insert(NameRole, "name");
    roles.insert(TotalBalanceRole, "total_balance");
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

void QMLDebtorModel::processResult(const QueryResult &result)
{
    if (result.request().parent() != this)
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

}
