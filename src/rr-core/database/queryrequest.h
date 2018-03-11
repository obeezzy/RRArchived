#ifndef QUERYREQUEST_H
#define QUERYREQUEST_H

#include <QObject>
#include <QVariantMap>
#include <QDebug>

class QueryRequest : public QObject
{
    Q_OBJECT
public:
    enum Type {
        Unknown,
        User,
        Dashboard,
        Stock,
        Sales
    }; Q_ENUM(Type)
    explicit QueryRequest(QObject *parent = nullptr); // NOTE: The parent parameter is mandatory!
    QueryRequest(const QueryRequest &other);
    QueryRequest &operator= (const QueryRequest &other);

    void setCommand(const QString &command, const QVariantMap &params, const Type type);
    QString command() const;
    QVariantMap params() const;
    Type type() const;

    friend QDebug operator<<(QDebug debug, const QueryRequest &request)
    {
        debug.nospace() << "QueryRequest(command=" << request.command()
                        << ", params=" << request.params()
                        << ", type=" << request.type() << ") ";

        return debug;
    }
private:
    QString m_command;
    QVariantMap m_params;
    Type m_type;
};

#endif // QUERYREQUEST_H
