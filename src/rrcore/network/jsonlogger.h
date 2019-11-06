#ifndef JSONLOGGER_H
#define JSONLOGGER_H

#include <QObject>
#include <QByteArray>

class JsonLogger : public QObject
{
public:
    explicit JsonLogger(QObject *parent = nullptr);

    void append(const QByteArray &json);
};

#endif // JSONLOGGER_H
