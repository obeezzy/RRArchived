#include "serverrequest.h"
#include "user/userprofile.h"

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

ServerRequest::ServerRequest(const ServerRequest &other) :
    QObject (nullptr)
{
    setAction(other.action());
    setData(other.data());
    setReceiver(other.receiver());
    m_queryRequest = other.queryRequest();
}

ServerRequest &ServerRequest::operator=(const ServerRequest &other)
{
    setAction(other.action());
    setData(other.data());
    setReceiver(other.receiver());
    m_queryRequest = other.queryRequest();

    return *this;
}

QObject *ServerRequest::receiver() const
{
    return m_receiver;
}

void ServerRequest::setReceiver(QObject *receiver)
{
    m_receiver = receiver;
}

QString ServerRequest::action() const
{
    return m_action;
}

void ServerRequest::setAction(const QString &action, const QVariantMap &data)
{
    m_action = action;
    if (m_data.isEmpty())
        m_data = data;
}

QVariantMap ServerRequest::data() const
{
    return m_data;
}

void ServerRequest::setData(const QVariantMap &data)
{
    m_data = data;
}

void ServerRequest::setQueryRequest(const QueryRequest &queryRequest)
{
    m_queryRequest = queryRequest;
}

QueryRequest ServerRequest::queryRequest() const
{
    return m_queryRequest;
}

ServerRequest ServerRequest::fromJson(const QByteArray &json)
{
    ServerRequest request;
    const QJsonObject serverRequestObject{ QJsonDocument::fromJson(json).object() };
    if (serverRequestObject.contains("action")) {
        request.setAction(serverRequestObject.value("action").toString());
        request.setData(serverRequestObject.value("data").toObject().toVariantMap());
    } else if (serverRequestObject.contains("command")) {
        request.setQueryRequest(QueryRequest::fromJson(QJsonDocument(serverRequestObject).toJson()));
    }

    return request;
}

QByteArray ServerRequest::toJson() const
{
    QJsonObject serverRequestObject;
    QJsonObject queryRequestObject;

    if (!m_queryRequest.command().isEmpty()) {
        queryRequestObject = QJsonDocument::fromJson(m_queryRequest.toJson()).object();
        serverRequestObject = queryRequestObject;
    }

    if (!m_action.isEmpty())
        serverRequestObject.insert("action", m_action);
    if (!m_data.isEmpty())
        serverRequestObject.insert("data", QJsonObject::fromVariantMap(m_data));

    return QJsonDocument(serverRequestObject).toJson();
}
