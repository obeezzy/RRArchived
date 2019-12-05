#include "requestlogger.h"

#include <QStandardPaths>
#include <QDir>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QSettings>
#include <QLoggingCategory>
#include <QDebug>

#include "serverrequest.h"

Q_LOGGING_CATEGORY(requestLogger, "rrcore.network.requestlogger", QtWarningMsg);

const QString BACKUP_LOCATION = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/RecordRack/backup";
const QString BACKUP_FILEPATH = BACKUP_LOCATION + "/rr.json";
const int MAX_BACKUP_SIZE = 1024 * 1000 * 5; // 5 MB

RequestLogger::RequestLogger(QObject *parent) :
    QObject(parent)
{
    QDir(BACKUP_LOCATION).mkpath(BACKUP_LOCATION);
    m_backupArray = readBackupArray();
}

void RequestLogger::push(const ServerRequest &request)
{
    const QJsonObject &jsonObject{ QJsonDocument::fromJson(request.toJson()).object() };

    m_backupArray.append(jsonObject);

    writeBackupArray();
}

bool RequestLogger::hasNext() const
{
    return !m_backupArray.isEmpty();
}

ServerRequest RequestLogger::nextRequest()
{
    if (!m_backupArray.isEmpty())
        return ServerRequest::fromJson(QJsonDocument(m_backupArray.first().toObject()).toJson());

    return ServerRequest();
}

void RequestLogger::pop()
{
    if (!m_backupArray.isEmpty())
        m_backupArray.removeFirst();

    writeBackupArray();
}

bool RequestLogger::isFull() const
{
    return QJsonDocument(m_backupArray).toJson().size() >= MAX_BACKUP_SIZE;
}

bool RequestLogger::dumpRequired() const
{
    return m_settings.value("sql_dump_needed").toBool();
}

QJsonArray RequestLogger::readBackupArray() const
{
    if (!m_settings.value("sql_dump_needed").toBool())
        return QJsonArray();

    QFile file(BACKUP_FILEPATH);
    file.open(QFile::ReadOnly);

    if (isFull()) {
        qCWarning(requestLogger) << "Backup size is larger than" << MAX_BACKUP_SIZE << "MB.";
    }

    return QJsonDocument::fromJson(file.readAll()).array();
}

void RequestLogger::writeBackupArray()
{
    QFile file(BACKUP_FILEPATH);
    file.open(QFile::WriteOnly);

    if (file.size() > MAX_BACKUP_SIZE) {
        qWarning(requestLogger) << "Backup size is larger than" << MAX_BACKUP_SIZE << "MB.";
        m_settings.setValue("sql_dump_needed", true);
        return;
    }

    file.write(QJsonDocument(m_backupArray).toJson());
}
