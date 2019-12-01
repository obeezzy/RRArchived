#include "serverresponse.h"
#include "serverrequest.h"
#include <QJsonObject>
#include <QJsonDocument>
#include "database/queryresult.h"
#include "database/databaseerror.h"
#include "network/networkerror.h"

ServerResponse::ServerResponse(QObject *parent) :
    QObject(parent),
    m_request(ServerRequest()),
    m_successful(false),
    m_errorCode(-1),
    m_statusCode(200)
{
    qRegisterMetaType<ServerResponse>("ServerResponse");
}

ServerResponse::ServerResponse(const ServerRequest &request) :
    QObject(nullptr),
    m_request(request),
    m_successful(false),
    m_errorCode(-1),
    m_statusCode(200)
{
    qRegisterMetaType<ServerResponse>("ServerResponse");
}

ServerResponse::ServerResponse(const QueryResult &queryResult) :
    QObject(nullptr),
    m_queryResult(queryResult),
    m_successful(false),
    m_errorCode(-1),
    m_statusCode(200)
{
    qRegisterMetaType<ServerResponse>("ServerResponse");
}

ServerResponse::ServerResponse(const ServerResponse &other) :
    QObject(nullptr)
{
    setSuccessful(other.isSuccessful());
    setQueryResult(other.queryResult());
    setErrorCode(other.errorCode());
    setErrorMessage(other.errorMessage());
    setStatusCode(other.statusCode());
    setStatusMessage(other.statusMessage());
    setRequest(other.request());
    setServerErrorCode(other.serverErrorCode());
    setData(other.data());
}

ServerResponse &ServerResponse::operator=(const ServerResponse &other)
{
    setSuccessful(other.isSuccessful());
    setQueryResult(other.queryResult());
    setErrorCode(other.errorCode());
    setErrorMessage(other.errorMessage());
    setStatusCode(other.statusCode());
    setStatusMessage(other.statusMessage());
    setRequest(other.request());
    setServerErrorCode(other.serverErrorCode());
    setData(other.data());

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

bool ServerResponse::isValid() const
{
    return !m_request.action().trimmed().isEmpty() || !m_queryResult.request().command().trimmed().isEmpty();
}

QVariantMap ServerResponse::data() const
{
    return m_data;
}

void ServerResponse::setData(const QVariantMap &data)
{
    m_data = data;
}

bool ServerResponse::hasError() const
{
    return m_errorCode != -1 && !m_errorMessage.trimmed().isEmpty();
}

int ServerResponse::errorCode() const
{
    return m_errorCode;
}

void ServerResponse::setErrorCode(int errorCode)
{
    m_errorCode = errorCode;
}

QString ServerResponse::serverErrorCode() const
{
    return m_serverErrorCode;
}

void ServerResponse::setServerErrorCode(const QString &serverErrorCode)
{
    m_serverErrorCode = serverErrorCode;
}

QString ServerResponse::errorMessage() const
{
    return m_errorMessage;
}

void ServerResponse::setErrorMessage(const QString &errorMessage)
{
    m_errorMessage = errorMessage;
}

int ServerResponse::statusCode() const
{
    return m_statusCode;
}

void ServerResponse::setStatusCode(int statusCode)
{
    m_statusCode = statusCode;
}

QString ServerResponse::statusMessage() const
{
    return m_statusMessage;
}

void ServerResponse::setStatusMessage(const QString &statusMessage)
{
    m_statusMessage = statusMessage;
}

ServerRequest ServerResponse::request() const
{
    return m_request;
}

void ServerResponse::setRequest(const ServerRequest &request)
{
    m_request = request;
}

QueryResult ServerResponse::queryResult() const
{
    return m_queryResult;
}

void ServerResponse::setQueryResult(const QueryResult &queryResult)
{
    m_queryResult = queryResult;
}

ServerResponse ServerResponse::fromJson(const QByteArray &json, const ServerRequest &request)
{
    ServerResponse response;
    QJsonObject jsonObject{ QJsonDocument::fromJson(json).object() };

    response.setSuccessful(jsonObject.value("successful").toBool());
    response.setRequest(request);
    response.setData(jsonObject.value("data").toObject().toVariantMap());
    response.setServerErrorCode(jsonObject.value("error").toObject().value("code").toString());
    response.setErrorCode(serverErrorCodeAsInteger(jsonObject.value("error").toObject().value("code").toString()));
    response.setErrorMessage(jsonObject.value("error").toObject().value("message").toString());

    return response;
}

ServerResponse ServerResponse::fromJson(const QByteArray &json, const QueryRequest &request)
{
    ServerResponse response;
    QJsonObject jsonObject{ QJsonDocument::fromJson(json).object() };

    response.setSuccessful(jsonObject.value("successful").toBool());
    response.setData(jsonObject.value("data").toObject().toVariantMap());
    response.setServerErrorCode(jsonObject.value("error").toObject().value("code").toString());
    response.setErrorCode(queryErrorCodeAsInteger(jsonObject.value("error").toObject().value("code").toString()));
    response.setErrorMessage(jsonObject.value("error").toObject().value("message").toString());

    const QJsonObject queryResultObject {
        { "successful", jsonObject.value("successful") },
        { "outcome", jsonObject.value("outcome") }
    };
    QueryResult result(QueryResult::fromJson(QJsonDocument(queryResultObject).toJson(), request));
    result.setErrorCode(response.errorCode());
    result.setErrorMessage(response.errorMessage());
    response.setQueryResult(result);

    return response;
}

int ServerResponse::serverErrorCodeAsInteger(const QString &errorCode)
{
    if (errorCode.trimmed().isEmpty())
        return -1;

    NetworkError::ServerErrorCode code = NetworkError::serverErrorHash.value(errorCode,
                                                                             NetworkError::ServerErrorCode::UnknownError);
    return static_cast<int>(code);
}

int ServerResponse::queryErrorCodeAsInteger(const QString &errorCode)
{
    if (errorCode.trimmed().isEmpty())
        return -1;

    DatabaseError::QueryErrorCode code = DatabaseError::queryErrorHash.value(errorCode,
                                                                             DatabaseError::QueryErrorCode::UnknownError);
    return static_cast<int>(code);
}
