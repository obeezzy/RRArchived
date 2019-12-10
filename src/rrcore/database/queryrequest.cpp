#include "queryrequest.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

QueryRequest::QueryRequest(QObject *receiver) :
    m_receiver(receiver),
    m_queryGroup(QueryGroup::Unknown)
{
    qRegisterMetaType<QueryRequest>("QueryRequest");
}

QueryRequest::QueryRequest(const QueryRequest &other) :
    QObject (nullptr),
    m_receiver(other.receiver()),
    m_queryGroup(QueryGroup::Unknown)
{
    setCommand(other.command(), other.params(), other.queryGroup());
}

QueryRequest &QueryRequest::operator=(const QueryRequest &other)
{
    setCommand(other.command(), other.params(), other.queryGroup());
    setReceiver(other.receiver());

    return *this;
}

bool QueryRequest::canUndo() const
{
    return m_params.value("can_undo").toBool();
}

bool QueryRequest::isUndoSet() const
{
    return m_command.startsWith("undo_");
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

void QueryRequest::setParams(const QVariantMap &params)
{
    m_params = params;
}

QVariantMap QueryRequest::params() const
{
    return m_params;
}

QueryRequest::QueryGroup QueryRequest::queryGroup() const
{
    return m_queryGroup;
}

QueryRequest::CommandVerb QueryRequest::commandVerb() const
{
    if (m_command.startsWith("sign")
             || m_command == "change_password")
        return CommandVerb::Authenticate;
    else if (m_command.startsWith("view")
             || m_command.startsWith("filter"))
        return CommandVerb::Read;
    else if (m_command.startsWith("update")
             || m_command.startsWith("change")
             || m_command.startsWith("deduct"))
        return CommandVerb::Update;
    else if (m_command.startsWith("archive")
             || m_command.startsWith("undo"))
        return CommandVerb::Delete;

    return CommandVerb::Create;
}

QByteArray QueryRequest::toJson() const
{
    QJsonObject jsonObject {
        { "command", m_command },
        { "params", QJsonObject::fromVariantMap(m_params) },
        { "query_group", queryGroupToString(m_queryGroup) }
    };

    return QJsonDocument(jsonObject).toJson();
}

QueryRequest QueryRequest::fromJson(const QByteArray &json)
{
    if (json.isEmpty())
        return QueryRequest();

    const QJsonObject &object = QJsonDocument::fromJson(json).object();
    QueryRequest request;
    request.setCommand(object.value("command").toString(),
                       object.value("params").toVariant().toMap(),
                       queryGroupToEnum(object.value("query_group").toString()));

    return request;
}

QueryRequest::QueryGroup QueryRequest::queryGroupToEnum(const QString &queryGroupString)
{
    if (queryGroupString == "stock")
        return QueryGroup::Stock;
    else if (queryGroupString == "sales")
        return QueryGroup::Sales;
    else if (queryGroupString == "purchase")
        return QueryGroup::Purchase;
    else if (queryGroupString == "income")
        return QueryGroup::Income;
    else if (queryGroupString == "expense")
        return QueryGroup::Expense;
    else if (queryGroupString == "debtor")
        return QueryGroup::Debtor;
    else if (queryGroupString == "user")
        return QueryGroup::User;
    else if (queryGroupString == "client")
        return QueryGroup::Client;
    else if (queryGroupString == "dashboard")
        return QueryGroup::Dashboard;

    return QueryGroup::Unknown;
}

QString QueryRequest::queryGroupToString(QueryRequest::QueryGroup queryGroupEnum)
{
    switch (queryGroupEnum) {
    case QueryGroup::Stock:
        return "stock";
    case QueryGroup::Sales:
        return "sales";
    case QueryGroup::Purchase:
        return "purchase";
    case QueryGroup::Income:
        return "income";
    case QueryGroup::Expense:
        return "expense";
    case QueryGroup::Debtor:
        return "debotr";
    case QueryGroup::User:
        return "user";
    case QueryGroup::Client:
        return "client";
    case QueryGroup::Dashboard:
        return "dashboard";
    default:
        break;
    }

    return QString();
}

void QueryRequest::setCommand(const QString &command, const QVariantMap &params, const QueryGroup queryGroup)
{
    m_command = command;
    m_params = params;
    m_queryGroup = queryGroup;
}
