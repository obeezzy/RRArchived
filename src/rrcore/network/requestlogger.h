#ifndef REQUESTLOGGER_H
#define REQUESTLOGGER_H

#include <QObject>
#include <QByteArray>
#include <QJsonArray>
#include <QSettings>
#include "serverrequest.h"

class RequestLogger : public QObject
{
public:
    explicit RequestLogger(QObject *parent = nullptr);

    bool dumpRequired() const;
    bool isFull() const;
    bool hasNext() const;

    void push(const ServerRequest &request);
    void pop();
    ServerRequest nextRequest();
private:
    QJsonArray m_backupArray;
    QSettings m_settings;

    QJsonArray readBackupArray() const;
    void writeBackupArray();
};

#endif // REQUESTLOGGER_H
