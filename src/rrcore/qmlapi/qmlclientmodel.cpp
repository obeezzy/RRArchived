#include "qmlclientmodel.h"

QMLClientModel::QMLClientModel(QObject *parent) :
    AbstractVisualListModel(parent)
{

}

QMLClientModel::QMLClientModel(DatabaseThread &thread) :
    AbstractVisualListModel(thread)
{

}

int QMLClientModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_records.count();
}

QVariant QMLClientModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case ClientIdRole:
        return m_records.at(index.row()).toMap().value("client_id").toString();
    case PreferredNameRole:
        return m_records.at(index.row()).toMap().value("preferred_name").toString();
    case PhoneNumberRole:
        return m_records.at(index.row()).toMap().value("phone_number").toString();
    }

    return QVariant();
}

QHash<int, QByteArray> QMLClientModel::roleNames() const
{
    QHash<int, QByteArray> roles(AbstractVisualListModel::roleNames());
    roles.insert(PreferredNameRole, "client_id");
    roles.insert(PreferredNameRole, "preferred_name");
    roles.insert(PhoneNumberRole, "phone_number");

    return roles;
}

void QMLClientModel::tryQuery()
{
    setBusy(true);
    QueryRequest request(this);
    request.setCommand("view_clients", QVariantMap(), QueryRequest::Client);
    emit executeRequest(request);
}

void QMLClientModel::processResult(const QueryResult result)
{
    if (result.request().receiver() != this)
        return;

    setBusy(false);

    if (result.isSuccessful()) {
        beginResetModel();
        m_records = result.outcome().toMap().value("clients").toList();
        endResetModel();

        emit success(ViewClientsSuccess);
    } else {
        emit error(UnknownError);
    }
}

void QMLClientModel::filter()
{
    if (filterColumn() == -1)
        return;

    setBusy(true);

    QueryRequest request(this);
    QVariantMap params;

    params.insert("filter_text", filterText());

    switch (filterColumn()) {
    case PhoneNumberColumn:
        params.insert("filter_column", "phone_number");
        break;
    default:
        params.insert("filter_column", "preferred_name");
        break;
    }

    request.setCommand("view_clients", params, QueryRequest::Client);
    emit executeRequest(request);
}
