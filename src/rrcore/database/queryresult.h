#ifndef QUERYRESULT_H
#define QUERYRESULT_H

#include <QObject>
#include <QSqlRecord>
#include <QDebug>
#include <QVariant>
#include "queryrequest.h"

class QueryResult : public QObject
{
    Q_OBJECT
public:
    explicit QueryResult(QObject *parent = nullptr);
    explicit QueryResult(const QueryRequest &request);
    QueryResult(const QueryResult &other);
    QueryResult &operator= (const QueryResult &other);

    void setSuccessful(bool);
    bool isSuccessful() const;

    void setRequest(const QueryRequest &request);
    const QueryRequest &request() const;

    void setErrorCode(int code);
    int errorCode() const;

    void setErrorMessage(const QString &message);
    QString errorMessage() const;

    void setErrorDetails(const QString &errorDetails);
    QString errorDetails() const;

    void setOutcome(const QVariant &outcome);
    QVariant outcome() const;

    static QueryResult fromJson(const QByteArray &json, const QueryRequest &request = QueryRequest());

    friend QDebug operator<<(QDebug debug, const QueryResult &result)
    {
        debug.nospace() << "QueryResult(command=" << result.request().command()
                        << ", successful=" << result.isSuccessful()
                        << ", errorCode=" << result.errorCode()
                        << ", errorMessage=" << result.errorMessage()
                        << ", errorDetails=" << result.errorDetails()
                        << ", recordCount=" << result.outcome().toMap().value("record_count").toInt()
                        << ", outcomeValid=" << result.outcome().isValid()
                        << ")";

        return debug;
    }
private:
    QueryRequest m_request;
    bool m_successful {false};
    int m_errorCode {-1};
    QString m_errorMessage;
    QString m_errorDetails;
    QVariant m_outcome;
};

#endif // QUERYRESULT_H
