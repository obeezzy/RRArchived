#include "userprivilegecenter.h"

#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QVariantMap>

const QString PRIVILEGES_FILE(":/json/privileges.json");

UserPrivilegeCenter::UserPrivilegeCenter(QObject *parent) :
    QObject(parent)
{

}

QVariant UserPrivilegeCenter::getPrivileges(const QString &group) const
{
    QFile file(PRIVILEGES_FILE);
    if (!file.open(QFile::ReadOnly))
        return QVariant();

    QJsonObject obj(QJsonDocument::fromJson(file.readAll()).object());

    if (group.trimmed().isEmpty())
        return obj.toVariantMap();

    return obj.toVariantMap().value(group);
}
