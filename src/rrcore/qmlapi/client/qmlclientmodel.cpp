#include "qmlclientmodel.h"
#include "database/databasethread.h"
#include "query/client/filterclients.h"
#include "query/client/viewclients.h"
#include "utility/common/filtercriteria.h"

QMLClientModel::QMLClientModel(QObject* parent)
    : AbstractVisualListModel(DatabaseThread::instance(), parent)
{}

QMLClientModel::QMLClientModel(DatabaseThread& thread, QObject* parent)
    : AbstractVisualListModel(thread, parent)
{}

int QMLClientModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;

    return m_clientList.count();
}

QVariant QMLClientModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
        case ClientIdRole:
            return m_clientList.at(index.row()).id;
        case PreferredNameRole:
            return m_clientList.at(index.row()).preferredName;
        case PhoneNumberRole:
            return m_clientList.at(index.row()).phoneNumber;
    }

    return QVariant();
}

QHash<int, QByteArray> QMLClientModel::roleNames() const
{
    return {{PreferredNameRole, "client_id"},
            {PreferredNameRole, "preferred_name"},
            {PhoneNumberRole, "phone_number"}};
}

void QMLClientModel::tryQuery()
{
    setBusy(true);
    emit execute(new Query::Client::ViewClients(this));
}

bool QMLClientModel::canProcessResult(const QueryResult& result) const
{
    Q_UNUSED(result)
    return true;
}

void QMLClientModel::processResult(const QueryResult& result)
{
    setBusy(false);

    if (result.isSuccessful()) {
        beginResetModel();
        m_clientList = Utility::ClientList{
            result.outcome().toMap().value("clients").toList()};
        endResetModel();

        emit success(ModelResult{ViewClientsSuccess});
    } else {
        emit error();
    }
}

void QMLClientModel::filter()
{
    if (filterColumn() == -1)
        return;

    setBusy(true);
    emit execute(new Query::Client::FilterClients(
        filterCriteria(), Utility::RecordGroup::None, this));
}

QString QMLClientModel::columnName(int column) const
{
    switch (column) {
        case PhoneNumberColumn:
            return QStringLiteral("phone_number");
        case PreferredNameColumn:
            return QStringLiteral("preferred_name");
    }

    return QString();
}
