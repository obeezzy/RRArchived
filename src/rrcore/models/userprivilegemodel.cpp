#include "userprivilegemodel.h"

UserPrivilegeModel::UserPrivilegeModel(QObject *parent) :
    AbstractVisualListModel (parent)
{

}

UserPrivilegeModel::UserPrivilegeModel(DatabaseThread &thread) :
    AbstractVisualListModel (thread)
{

}

int UserPrivilegeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_records.count();
}

QVariant UserPrivilegeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case KeyRole:
        return m_records.at(index.row()).toMap().value("key").toString();
    case ShortDescriptionRole:
        return m_records.at(index.row()).toMap().value("short_description").toString();
    case LongDescriptionRole:
        return m_records.at(index.row()).toMap().value("long_description").toString();
    case ValueRole:
        return m_records.at(index.row()).toMap().value("value").toBool();
    }

    return QVariant();
}

QHash<int, QByteArray> UserPrivilegeModel::roleNames() const
{
    return {
        { KeyRole, "key" },
        { ShortDescriptionRole, "short_description" },
        { LongDescriptionRole, "long_description" },
        { ValueRole, "value" }
    };
}

bool UserPrivilegeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    switch (role) {
    case ValueRole:
        QVariantMap privilege = m_records.at(index.row()).toMap();
        privilege.insert("value", value);
        m_records.replace(index.row(), privilege);
        emit dataChanged(index, index);
        break;
    }

    return true;
}

void UserPrivilegeModel::setPrivileges(const QString &privilegeGroup, const QString &title, const QVariantList &privileges)
{
    m_privilegeGroup = privilegeGroup;
    m_title = title;
    m_records = privileges;
}

void UserPrivilegeModel::setPrivilegeValue(int row, bool value)
{
    if (row < 0 || row >= rowCount())
        return;

    QVariantMap privilege = m_records.at(row).toMap();
    privilege.insert("value", value);
    m_records.replace(row, privilege);
}

QString UserPrivilegeModel::privilegeGroup() const
{
    return m_privilegeGroup;
}

QString UserPrivilegeModel::title() const
{
    return m_title;
}

QVariantList UserPrivilegeModel::privileges() const
{
    return m_records;
}

void UserPrivilegeModel::tryQuery()
{

}

void UserPrivilegeModel::processResult(const QueryResult result)
{
    Q_UNUSED(result)
}
