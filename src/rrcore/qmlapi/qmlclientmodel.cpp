#include "qmlclientmodel.h"
#include "database/databasethread.h"
#include "queryexecutors/client.h"

QMLClientModel::QMLClientModel(QObject *parent) :
    AbstractVisualListModel(DatabaseThread::instance(), parent)
{}

QMLClientModel::QMLClientModel(DatabaseThread &thread, QObject *parent) :
    AbstractVisualListModel(thread, parent)
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
    return {
        { PreferredNameRole, "client_id" },
        { PreferredNameRole, "preferred_name" },
        { PhoneNumberRole, "phone_number" }
    };
}

void QMLClientModel::tryQuery()
{
    setBusy(true);
    emit execute(new ClientQuery::ViewClients(this));
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
    emit execute(new ClientQuery::ViewClients(
                     filterText(),
                     columnName(),
                     this));
}

QString QMLClientModel::columnName() const
{
    switch (filterColumn()) {
    case PhoneNumberColumn:
        return QStringLiteral("phone_number");
    case PreferredNameColumn:
        return QStringLiteral("preferred_name");
    }

    return QString();
}
