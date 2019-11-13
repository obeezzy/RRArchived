#include "serverrequest.h"
#include "singletons/userprofile.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

ServerRequest::ServerRequest(QObject *receiver) :
    m_receiver(receiver)
{
    qRegisterMetaType<ServerRequest>("ServerRequest");
}

ServerRequest::ServerRequest(const QueryRequest &queryRequest) :
    m_queryRequest(queryRequest)
{

}

QObject *ServerRequest::receiver() const
{
    return m_receiver;
}

void ServerRequest::setReceiver(QObject *receiver)
{
    m_receiver = receiver;
}

QueryRequest ServerRequest::queryRequest() const
{
    return m_queryRequest;
}

QByteArray ServerRequest::toJson() const
{
    const QJsonObject &queryRequestObject { QJsonDocument::fromJson(m_queryRequest.toJson()).object() };
    QJsonObject serverRequestObject { queryRequestObject };
    serverRequestObject.insert("userId", UserProfile::instance().userId());
    serverRequestObject.insert("rackId", UserProfile::instance().rackId());

    return QJsonDocument(serverRequestObject).toJson();
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
