#ifndef SERVERREQUEST_H
#define SERVERREQUEST_H

#include <QObject>
#include <QDebug>
#include "database/queryrequest.h"

class ServerRequest : public QObject
{
    Q_OBJECT
public:
    explicit ServerRequest(QObject *parent = nullptr);
    explicit ServerRequest(const QueryRequest &queryRequest);
    ServerRequest(const ServerRequest &other);
    ServerRequest &operator=(const ServerRequest &other);

    QObject *receiver() const;
    void setReceiver(QObject *receiver);

    QueryRequest queryRequest() const;
    QByteArray toJson() const;

    friend QDebug operator<<(QDebug debug, const ServerRequest &request)
    {
        Q_UNUSED(request)
        debug.nospace() << "ServerRequest(command=) ";

        return debug;
    }
private:
    QObject *m_receiver;
    QueryRequest m_queryRequest;
};

#endif // SERVERREQUEST_H
