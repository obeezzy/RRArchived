#include "queryrequest.h"

QueryRequest::QueryRequest(QObject *receiver) :
    QObject(nullptr),
    m_receiver(receiver)
{
    qRegisterMetaType<QueryRequest>("QueryRequest");
}

QueryRequest::QueryRequest(const QueryRequest &other)
    : QObject(nullptr)
{
    setCommand(other.command(), other.params(), other.type());
    setReceiver(other.receiver());
}

QueryRequest &QueryRequest::operator=(const QueryRequest &other)
{
    setCommand(other.command(), other.params(), other.type());
    setReceiver(other.receiver());

    return *this;
}

QObject *QueryRequest::receiver() const
{
    return m_receiver;
}

void QueryRequest::setReceiver(QObject *receiver)
{
    m_receiver = receiver;
}

QString QueryRequest::command() const
{
    return m_command;
}

QVariantMap QueryRequest::params() const
{
    return m_params;
}

QueryRequest::Type QueryRequest::type() const
{
    return m_type;
}

void QueryRequest::setCommand(const QString &command, const QVariantMap &params, const Type type)
{
    m_command = command;
    m_params = params;
    m_type = type;
}
