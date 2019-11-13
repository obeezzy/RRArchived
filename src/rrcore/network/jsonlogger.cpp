#include "jsonlogger.h"

#include <QStandardPaths>
#include <QDir>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>

#include "serverrequest.h"

const QString BACKUP_LOCATION = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/RecordRack/backup";
const QString BACKUP_FILEPATH = BACKUP_LOCATION + "/rr.json";
const int MAX_BACKUP_SIZE = 1024 * 1000 * 5; // 5 MB

JsonLogger::JsonLogger(QObject *parent) :
    QObject(parent)
{
    QDir(BACKUP_LOCATION).mkpath(BACKUP_LOCATION);
}

void JsonLogger::append(const QByteArray &json)
{
    QFile file(BACKUP_FILEPATH);
    file.open(QFile::ReadWrite);

    if (file.size() > MAX_BACKUP_SIZE) {
        qWarning() << "Backup size is larger than 5 MB.";
    }

    QJsonArray backupArray(QJsonDocument::fromJson(file.readAll()).array());
    const QJsonObject jsonObject(QJsonDocument::fromJson(json).object());

    backupArray.append(jsonObject);
    file.write(QJsonDocument(backupArray).toJson());
}

QList<ServerRequest> JsonLogger::requests() const
{
    return m_requests;
}
