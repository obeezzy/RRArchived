#include "qmlusermodel.h"
#include "database/databasethread.h"
#include "models/abstractvisualtablemodel.h"

const int COLUMN_COUNT = 5;

QMLUserModel::QMLUserModel(QObject *parent) :
    QMLUserModel(DatabaseThread::instance(), parent)
{}

QMLUserModel::QMLUserModel(DatabaseThread &thread, QObject *parent) :
    AbstractVisualTableModel(thread, parent)
{

}

int QMLUserModel::keys() const
{
    return m_keys;
}

void QMLUserModel::setKeys(int keys)
{
    if (m_keys == keys)
        return;

    m_keys = keys;
    emit keysChanged();
}

int QMLUserModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_records.count();
}

int QMLUserModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return COLUMN_COUNT;
}

QVariant QMLUserModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case UserIdRole:
        return m_records.at(index.row()).toMap().value("user_id").toInt();
    case UserRole:
        return m_records.at(index.row()).toMap().value("user").toString();
    case ActiveRole:
        return m_records.at(index.row()).toMap().value("active").toBool();
    case PresetRole:
        return m_records.at(index.row()).toMap().value("preset").toString();
    }

    return QVariant();
}

QHash<int, QByteArray> QMLUserModel::roleNames() const
{
    return {
        { UserIdRole, "user_id" },
        { UserRole, "user" },
        { ActiveRole, "active" },
        { PresetRole, "preset" }
    };
}

void QMLUserModel::tryQuery()
{
    QueryRequest request(this);
    request.setCommand("view_users", { }, QueryRequest::User);
    emit executeRequest(request);
}

void QMLUserModel::processResult(const QueryResult result)
{
    if (this != result.request().receiver())
        return;

    setBusy(false);
    if (result.isSuccessful()) {
        if (result.request().command() == "activate_user") {
            emit success(ActivateUserSuccess);
        } else {
            beginResetModel();
            if (result.request().command() == "view_users") {
                m_records = result.outcome().toMap().value("users").toList();
                emit success(ViewUsersSuccess);
            } else if (result.request().command() == "remove_user") {
                removeUserFromModel(result.request().params().value("user_name").toString());
                emit success(RemoveUserSuccess);
            } else if (result.request().command() == "undo_remove_user") {
                emit success(UndoRemoveUserSuccess);
            }
            endResetModel();
        }
    } else {
        emit error();
    }
}

void QMLUserModel::removeUserFromModel(const QString &userName)
{
    if (userName.trimmed().isEmpty())
        return;

    for (int i = 0; i < m_records.count(); ++i) {
        if (userName == m_records.at(i).toMap().value("user_name").toString()) {
            m_records.removeAt(i);
            break;
        }
    }
}

void QMLUserModel::removeUser(const QString &userName)
{
    setBusy(true);

    QueryRequest request(this);
    request.setCommand("remove_user", { { "user_name", userName } }, QueryRequest::User);
    emit executeRequest(request);
}

void QMLUserModel::activateUser(const QString &userName, bool active)
{
    setBusy(true);

    QueryRequest request(this);
    request.setCommand("activate_user", { { "user_name", userName }, { "active", active } }, QueryRequest::User);
    emit executeRequest(request);
}
