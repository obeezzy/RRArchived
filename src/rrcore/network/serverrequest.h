#ifndef SERVERREQUEST_H
#define SERVERREQUEST_H

#include <QObject>
#include <QDebug>

class ServerRequest : public QObject
{
    Q_OBJECT
public:
    explicit ServerRequest(QObject *parent = nullptr);
    ServerRequest(const ServerRequest &other);
    ServerRequest &operator=(const ServerRequest &other);

    QObject *receiver() const;
    void setReceiver(QObject *receiver);

    friend QDebug operator<<(QDebug debug, const ServerRequest &request)
    {
        Q_UNUSED(request)
        debug.nospace() << "ServerRequest(command=) ";

        return debug;
    }
private:
    QObject *m_receiver;
};

#endif // SERVERREQUEST_H
