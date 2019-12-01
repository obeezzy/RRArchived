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

    QString action() const;
    void setAction(const QString &action, const QVariantMap &data = QVariantMap());

    QVariantMap data() const;
    void setData(const QVariantMap &data);

    void setQueryRequest(const QueryRequest &queryRequest);
    QueryRequest queryRequest() const;

    static ServerRequest fromJson(const QByteArray &json);
    QByteArray toJson() const;

    friend QDebug operator<<(QDebug debug, const ServerRequest &request)
    {
        if (!request.queryRequest().command().trimmed().isEmpty()) {
            debug.nospace() << "ServerRequest("
                            << "command=" << request.queryRequest().command()
                            << ", params=" << request.queryRequest().params()
                            << ")";
        } else {
            debug.nospace() << "ServerRequest("
                            << "action=" << request.action()
                            << ", data=" << request.data()
                            << ")";
        }

        return debug;
    }
private:
    QObject *m_receiver;
    QueryRequest m_queryRequest;
    QString m_action;
    QVariantMap m_data;
};

#endif // SERVERREQUEST_H
