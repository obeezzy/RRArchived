#ifndef QUERYREQUEST_H
#define QUERYREQUEST_H

#include <QObject>
#include <QVariantMap>
#include <QDebug>

class QueryRequest : public QObject
{
    Q_OBJECT
public:
    enum class QueryGroup {
        Unknown,
        Client,
        User,
        Dashboard,
        Stock,
        Sales,
        Purchase,
        Income,
        Expense,
        Debtor
    }; Q_ENUM(QueryGroup)

    enum class CommandVerb {
        Create,
        Read,
        Update,
        Delete,
        Authenticate
    }; Q_ENUM(CommandVerb)

    explicit QueryRequest(QObject *receiver = nullptr); // NOTE: The parent parameter is mandatory!
    QueryRequest(const QueryRequest &other);
    QueryRequest &operator= (const QueryRequest &other);

    inline bool operator ==(const QueryRequest &other) const {
        return m_command == other.command() && m_queryGroup == other.queryGroup();
    }

    bool canUndo() const;
    bool isUndoSet() const;

    QObject *receiver() const;
    void setReceiver(QObject *receiver);

    void setCommand(const QString &command, const QVariantMap &params, const QueryGroup queryGroup);
    QString command() const;

    void setParams(const QVariantMap &params);

    QVariantMap params() const;
    QueryGroup queryGroup() const;
    CommandVerb commandVerb() const;
    QByteArray toJson() const;

    static QueryRequest fromJson(const QByteArray &json);

    friend QDebug operator<<(QDebug debug, const QueryRequest &request)
    {
        debug.nospace() << "QueryRequest(command=" << request.command()
                        << ", params=" << request.params()
                        << ", queryGroup=" << request.queryGroup()
                        << ")";

        return debug;
    }
private:
    QObject *m_receiver;
    QString m_command;
    QVariantMap m_params;
    QueryGroup m_queryGroup;

    static QueryGroup queryGroupToEnum(const QString &queryGroupString);
    static QString queryGroupToString(QueryGroup queryGroupEnum);
};

#endif // QUERYREQUEST_H
