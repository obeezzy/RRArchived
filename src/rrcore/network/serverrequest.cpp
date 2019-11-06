#include "serverrequest.h"

ServerRequest::ServerRequest(QObject *receiver) :
    m_receiver(receiver)
{
    qRegisterMetaType<ServerRequest>("ServerRequest");
}

QObject *ServerRequest::receiver() const
{
    return m_receiver;
}

void ServerRequest::setReceiver(QObject *receiver)
{
    m_receiver = receiver;
}

ServerRequest::ServerRequest(const ServerRequest &other) :
    QObject (nullptr),
    m_receiver(other.receiver())
{
    //setCommand(other.command(), other.params(), other.type());
}

ServerRequest &ServerRequest::operator=(const ServerRequest &other)
{
    //setCommand(other.command(), other.params(), other.type());
    setReceiver(other.receiver());

    return *this;
}
