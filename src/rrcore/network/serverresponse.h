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
    explicit ServerResponse(const QueryResult &queryResult);
    ServerResponse(const ServerResponse &other);
    ServerResponse &operator= (const ServerResponse &other);

    friend QDebug operator<<(QDebug debug, const ServerResponse &response)
    {
        if (!response.queryResult().request().command().trimmed().isEmpty()) {
            debug.nospace() << "ServerResponse(successful=" << response.isSuccessful()
                            << ", " << response.queryResult() << ")";
        } else if (!response.request().action().trimmed().isEmpty()) {
            debug.nospace() << "ServerResponse(successful=" << response.isSuccessful()
                            << ", action=" << response.request().action()
                            << ", data=" << response.data()
                            << ", errorCode=" << response.errorCode()
                            << ", errorMessage=" << response.errorMessage()
                            << ", serverErrorCode=" << response.serverErrorCode()
                            << ")";
        } else {
            debug.nospace() << "ServerResponse(Invalid)";
        }
        return debug;
    }

    bool isSuccessful() const;
    void setSuccessful(bool successful);

    bool isValid() const;

    QVariantMap data() const;
    void setData(const QVariantMap &data);

    bool hasError() const;

    int errorCode() const;
    void setErrorCode(int errorCode);

    QString serverErrorCode() const;
    void setServerErrorCode(const QString &serverErrorCode);

    QString errorMessage() const;
    void setErrorMessage(const QString &errorMessage);

    int statusCode() const;
    void setStatusCode(int statusCode);

    QString statusMessage() const;
    void setStatusMessage(const QString &statusMessage);

    ServerRequest request() const;
    void setRequest(const ServerRequest &request);

    QueryResult queryResult() const;
    void setQueryResult(const QueryResult &queryResult);

    static ServerResponse fromJson(const QByteArray &json, const ServerRequest &request = ServerRequest());
    static ServerResponse fromJson(const QByteArray &json, const QueryRequest &request);
private:
    ServerRequest m_request;
    QueryResult m_queryResult;
    bool m_successful;
    int m_errorCode;
    int m_statusCode;
    QString m_errorMessage;
    QVariantMap m_data;
    QString m_statusMessage;
    QString m_serverErrorCode;

    static int serverErrorCodeAsInteger(const QString &errorCode);
    static int queryErrorCodeAsInteger(const QString &errorCode);
};

#endif // SERVERRESPONSE_H
