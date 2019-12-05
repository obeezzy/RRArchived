#ifndef NETWORKTHREAD_H
#define NETWORKTHREAD_H

#include <QThread>
#include <QLoggingCategory>
#include "serverrequest.h"
#include "serverresponse.h"
#include "database/queryresult.h"

class QNetworkAccessManager;
class QNetworkReply;
class RequestLogger;
class QueryExecutor;

class NetworkWorker : public QObject
{
    Q_OBJECT
public:
    explicit NetworkWorker(QObject *parent = nullptr);
    ~NetworkWorker() = default;

    void execute(const QueryRequest request);
    void execute(const ServerRequest request);
signals:
    void resultReady(const QueryResult result);
    void responseReady(const ServerResponse response);
private:
    QNetworkAccessManager *m_networkManager;
    RequestLogger *m_requestLogger;

    QUrl determineUrl(const QueryRequest &request) const; // throws NetworkException
    QUrl determineUrl(const ServerRequest &request) const; // throws NetworkException
    void waitForFinished(QNetworkReply *reply);
    void flushLoggedRequests(); // throws NetworkException
};

class NetworkThread : public QThread
{
    Q_OBJECT
public:
    static NetworkThread &instance();
    virtual ~NetworkThread() override;

    NetworkThread(NetworkThread const &) = delete;
    void operator=(NetworkThread const &) = delete;

    void run() override final;
    void syncWithServer(const QueryResult result);
    void tunnelToServer(QueryExecutor *queryExecutor);
signals:
    void execute(const QueryRequest request);
    void execute(const ServerRequest request);
    void responseReady(const ServerResponse response);
    void resultReady(const QueryResult result);
private:
    explicit NetworkThread(QObject *parent = nullptr);
};

Q_DECLARE_LOGGING_CATEGORY(networkThread);

#endif // NETWORKTHREAD_H
