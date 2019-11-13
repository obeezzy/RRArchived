#ifndef JSONLOGGER_H
#define JSONLOGGER_H

#include <QObject>
#include <QByteArray>
#include "serverrequest.h"

class JsonLogger : public QObject
{
public:
    explicit JsonLogger(QObject *parent = nullptr);

    void append(const QByteArray &json);
    QList<ServerRequest> requests() const;
private:
    QList<ServerRequest> m_requests;
};

#endif // JSONLOGGER_H
