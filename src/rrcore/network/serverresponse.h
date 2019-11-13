#ifndef SERVERRESPONSE_H
#define SERVERRESPONSE_H

#include <QObject>
#include <QDebug>
#include "serverrequest.h"
#include "database/queryresult.h"

class ServerResponse : public QObject
{
    Q_OBJECT
public:
    explicit ServerResponse(QObject *parent = nullptr);
    explicit ServerResponse(const ServerRequest &request);
    ServerResponse(const ServerResponse &other);
    ServerResponse &operator= (const ServerResponse &other);

    friend QDebug operator<<(QDebug debug, const ServerResponse &response)
    {
        debug.nospace() << "ServerResponse(successful=" << response.isSuccessful()
                        << ", " << response.queryResult() << ")";
        return debug;
    }

    bool isSuccessful() const;
    void setSuccessful(bool successful);

    QString errorMessage() const;
    void setErrorMessage(const QString &errorMessage);

    QueryResult queryResult() const;
    void setQueryResult(const QueryResult &queryResult);

    static ServerResponse fromJson(const QByteArray &json);
private:
    ServerRequest m_request;
    QueryResult m_queryResult;
    bool m_successful;
    QString m_errorMessage;
};

#endif // SERVERRESPONSE_H
