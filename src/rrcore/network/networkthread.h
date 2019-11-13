#ifndef NETWORKTHREAD_H
#define NETWORKTHREAD_H

#include <QThread>
#include <QLoggingCategory>
#include "serverrequest.h"
#include "serverresponse.h"
#include "database/queryresult.h"

class QNetworkAccessManager;
class QNetworkReply;
class JsonLogger;

class NetworkWorker : public QObject
{
    Q_OBJECT
public:
    enum ErrorCode {
        RequestFailed
    };

    explicit NetworkWorker(QObject *parent = nullptr);
    ~NetworkWorker();

    void execute(const QueryRequest request);
    void execute(const ServerRequest request);
signals:
    void responseReady(const ServerResponse result);
private:
    QNetworkAccessManager *m_networkManager;
    JsonLogger *m_jsonLogger;

    void waitForFinished(QNetworkReply *reply);
    void flushBackup();
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
    void tunnelToServer(const QueryResult result);

    inline static const QString SERVER_URL = QStringLiteral("http://localhost:3000");
    inline static const QString STOCK_API_URL = SERVER_URL + QStringLiteral("/api/database/stock");
    inline static const QString SALES_API_URL = SERVER_URL + QStringLiteral("/api/database/sales");
    inline static const QString PURCHASE_API_URL = SERVER_URL + QStringLiteral("/api/database/purchase");
    inline static const QString INCOME_API_URL = SERVER_URL + QStringLiteral("/api/database/income");
    inline static const QString EXPENSE_API_URL = SERVER_URL + QStringLiteral("/api/database/expense");
    inline static const QString DEBTOR_API_URL = SERVER_URL + QStringLiteral("/api/database/debtor");
    inline static const QString CREDITOR_API_URL = SERVER_URL + QStringLiteral("/api/database/creditor");
    inline static const QString USER_API_URL = SERVER_URL + QStringLiteral("/api/database/user");
signals:
    void execute(const QueryRequest request);
    void execute(const ServerRequest request);
    void responseReady(const ServerResponse response);
private:
    explicit NetworkThread(QObject *parent = nullptr);
};

Q_DECLARE_LOGGING_CATEGORY(networkThread);

#endif // NETWORKTHREAD_H
