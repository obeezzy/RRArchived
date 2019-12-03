#include "queryresult.h"

#include <QJsonObject>
#include <QJsonDocument>

QueryResult::QueryResult(QObject *parent) :
    QObject(parent),
    m_successful(false),
    m_errorCode(-1)
{
    qRegisterMetaType<QueryResult>("QueryResult");
}

QueryResult::QueryResult(const QueryRequest &request) :
    QObject(nullptr),
    m_request(request),
    m_successful(false),
    m_errorCode(-1)
{
    qRegisterMetaType<QueryResult>("QueryResult");
}

QueryResult::QueryResult(const QueryResult &other) :
    QObject(nullptr),
    m_request(other.request()),
    m_successful(other.isSuccessful()),
    m_errorCode(other.errorCode()),
    m_errorMessage(other.errorMessage()),
    m_errorUserMessage(other.errorUserMessage()),
    m_outcome(other.outcome())
{
}

QueryResult &QueryResult::operator= (const QueryResult &other)
{
    setSuccessful(other.isSuccessful());
    setErrorCode(other.errorCode());
    setRequest(other.request());
    setErrorMessage(other.errorMessage());
    setErrorUserMessage(other.errorUserMessage());
    setOutcome(other.outcome());

    return *this;
}

void QueryResult::setSuccessful(bool successful)
{
    m_successful = successful;
}

bool QueryResult::isSuccessful() const
{
    return m_successful;
}

void QueryResult::setRequest(const QueryRequest &request)
{
    m_request = request;
}

const QueryRequest &QueryResult::request() const
{
    return m_request;
}

void QueryResult::setErrorCode(int code)
{
    m_errorCode = code;
}

int QueryResult::errorCode() const
{
    return m_errorCode;
}

void QueryResult::setErrorMessage(QString message)
{
    m_errorMessage = message;
}

QString QueryResult::errorMessage() const
{
    return m_errorMessage;
}

void QueryResult::setErrorUserMessage(QString userMessage)
{
    m_errorUserMessage = userMessage;
}

QString QueryResult::errorUserMessage() const
{
    return m_errorUserMessage;
}

void QueryResult::setOutcome(const QVariant &outcome)
{
    m_outcome = outcome;
}

QVariant QueryResult::outcome() const
{
    return m_outcome;
}

QueryResult QueryResult::fromJson(const QByteArray &json, const QueryRequest &request)
{
    QJsonObject jsonObject{ QJsonDocument::fromJson(json).object() };
    QueryResult result;
    result.setSuccessful(jsonObject.value("successful").toBool());
    result.setOutcome(jsonObject.value("outcome").toObject().toVariantMap());
    result.setErrorMessage(jsonObject.value("error").toObject().value("message").toString());
    result.setRequest(request);
    if (jsonObject.value("error").toObject().contains("errno")
            && jsonObject.value("error").toObject().value("errno").isDouble())
        result.setErrorCode(jsonObject.value("error").toObject().value("errno").toInt());

    return result;
}
