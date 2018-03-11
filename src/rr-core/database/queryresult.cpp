#include "queryresult.h"

QueryResult::QueryResult(QObject *parent)
    : m_successful(false)
    , m_errorCode(-1)
    , QObject(parent)
{
    qRegisterMetaType<QueryResult>("QueryResult");
}

QueryResult::QueryResult(const QueryRequest &request)
    : m_successful(false)
    , m_errorCode(-1)
    , m_request(request)
{
}

QueryResult::QueryResult(const QueryResult &other)
{
    setSuccessful(other.isSuccessful());
    setErrorCode(other.errorCode());
    setRequest(other.request());
    setErrorMessage(other.errorMessage());
    setErrorUserMessage(other.errorUserMessage());
    setOutcome(other.outcome());
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
