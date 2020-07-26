#include "messagecenter.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantMap>

const QString MESSAGE_FILE(":/json/messages.json");

MessageCenter::MessageCenter(QObject* parent) : QObject(parent) {}

QVariant MessageCenter::getMessage(const QString& key) const
{
    if (key.trimmed().isEmpty())
        return QVariant();

    QFile file(MESSAGE_FILE);
    if (!file.open(QFile::ReadOnly))
        return QString();

    QJsonObject obj(QJsonDocument::fromJson(file.readAll()).object());

    return obj.toVariantMap().value(key);
}
