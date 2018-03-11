#include "queryrequest.h"

QueryRequest::QueryRequest(QObject *parent)
    : QObject(parent)
{
    qRegisterMetaType<QueryRequest>("QueryRequest");
}

QueryRequest::QueryRequest(const QueryRequest &other)
{
    setCommand(other.command(), other.params(), other.type());
    setParent(other.parent());
}

QueryRequest &QueryRequest::operator=(const QueryRequest &other)
{
    setCommand(other.command(), other.params(), other.type());
    setParent(other.parent());

    return *this;
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
