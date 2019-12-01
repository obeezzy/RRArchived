#include "queryrequest.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

QueryRequest::QueryRequest(QObject *receiver) :
    m_receiver(receiver)
{
    qRegisterMetaType<QueryRequest>("QueryRequest");
}

QueryRequest::QueryRequest(const QueryRequest &other) :
    QObject (nullptr),
    m_receiver(other.receiver())
{
    setCommand(other.command(), other.params(), other.type());
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
        { "type", typeEnumToString(m_type) }
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
                       typeStringToEnum(object.value("type").toString()));

    return request;
}

QueryRequest::Type QueryRequest::typeStringToEnum(const QString &typeString)
{
    if (typeString == "stock")
        return Stock;
    else if (typeString == "sales")
        return Sales;
    else if (typeString == "purchase")
        return Purchase;
    else if (typeString == "income")
        return Income;
    else if (typeString == "expense")
        return Expense;
    else if (typeString == "debtor")
        return Debtor;
    else if (typeString == "user")
        return User;
    else if (typeString == "client")
        return Client;
    else if (typeString == "dashboard")
        return Dashboard;

    return Unknown;
}

QString QueryRequest::typeEnumToString(QueryRequest::Type typeEnum)
{
    switch (typeEnum) {
    case Stock:
        return "stock";
    case Sales:
        return "sales";
    case Purchase:
        return "purchase";
    case Income:
        return "income";
    case Expense:
        return "expense";
    case Debtor:
        return "debotr";
    case User:
        return "user";
    case Client:
        return "client";
    case Dashboard:
        return "dashboard";
    case Unknown:
        return QString();
    }

    return QString();
}

void QueryRequest::setCommand(const QString &command, const QVariantMap &params, const Type type)
{
    m_command = command;
    m_params = params;
    m_type = type;
}
