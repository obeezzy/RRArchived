#include "serverresponse.h"
#include "serverrequest.h"
#include <QJsonObject>
#include <QJsonDocument>
#include "database/queryresult.h"

ServerResponse::ServerResponse(QObject *parent) :
    QObject(parent),
    m_request(ServerRequest()),
    m_successful(false)
{
    qRegisterMetaType<ServerResponse>("ServerResponse");
}

ServerResponse::ServerResponse(const ServerRequest &request) :
    QObject(nullptr),
    m_request(request),
    m_successful(false)
{
    qRegisterMetaType<ServerResponse>("ServerResponse");
}

ServerResponse::ServerResponse(const ServerResponse &other) :
    QObject(nullptr)
{
    setSuccessful(other.isSuccessful());
    setQueryResult(other.queryResult());
    setErrorMessage(other.errorMessage());
}

ServerResponse &ServerResponse::operator=(const ServerResponse &other)
{
    setSuccessful(other.isSuccessful());
    setQueryResult(other.queryResult());
    setErrorMessage(other.errorMessage());

    return *this;
}

bool ServerResponse::isSuccessful() const
{
    return m_successful;
}

void ServerResponse::setSuccessful(bool successful)
{
    m_successful = successful;
}

QString ServerResponse::errorMessage() const
{
    return m_errorMessage;
}

void ServerResponse::setErrorMessage(const QString &errorMessage)
{
    m_errorMessage = errorMessage;
}

QueryResult ServerResponse::queryResult() const
{
    return m_queryResult;
}

void ServerResponse::setQueryResult(const QueryResult &queryResult)
{
    m_queryResult = queryResult;
}

ServerResponse ServerResponse::fromJson(const QByteArray &json)
{
    ServerResponse response;
    QJsonObject jsonObject{ QJsonDocument::fromJson(json).object() };

    if (json.isEmpty())
        return response;

    response.setSuccessful(jsonObject.value("successful").toBool());

    QJsonObject queryResultObject {
        { "successful", jsonObject.value("successful") },
        { "outcome", jsonObject.value("outcome") }
    };

    response.setQueryResult(QueryResult::fromJson(QJsonDocument(queryResultObject).toJson()));

    return response;
}
