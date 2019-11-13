#include "networkthread.h"
#include "serverresponse.h"
#include "networkexception.h"
#include "jsonlogger.h"

#include <QElapsedTimer>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QEventLoop>

Q_LOGGING_CATEGORY(networkThread, "rrcore.database.networkthread");

NetworkWorker::NetworkWorker(QObject *parent) :
    QObject(parent)
{
    m_networkManager = new QNetworkAccessManager(this);
    m_jsonLogger = new JsonLogger(this);
}

NetworkWorker::~NetworkWorker()
{
}

void NetworkWorker::execute(const QueryRequest request)
{
    qCInfo(networkThread) << "NetworkWorker->" << request;
    ServerResponse response;
    QNetworkRequest networkRequest;
    ServerRequest serverRequest(request);

    QElapsedTimer timer;
    timer.start();

    try {
        if (request.type() == QueryRequest::Stock)
            networkRequest.setUrl(QUrl(NetworkThread::STOCK_API_URL));
        else if (request.type() == QueryRequest::Sales)
            networkRequest.setUrl(QUrl(NetworkThread::SALES_API_URL));
        else
            return;

        networkRequest.setRawHeader("Content-Type", "application/json");
        networkRequest.setRawHeader("Content-Length", QByteArray::number(serverRequest.toJson().size()));

        flushBackup();

        QNetworkReply *networkReply = m_networkManager->post(networkRequest, serverRequest.toJson());
        waitForFinished(networkReply);
        if (networkReply->error() != QNetworkReply::NoError)
            throw NetworkException(networkReply->error(),
                                   networkReply->errorString(),
                                   "Failed to receive reply!");

        response = ServerResponse::fromJson(networkReply->readAll());
        if (!response.isSuccessful())
            throw NetworkException(RequestFailed,
                                   response.errorMessage(),
                                   "Request failed.");

        QueryResult result{ response.queryResult() };
        result.setRequest(request);
        response.setQueryResult(result);

        qDebug() << "Response? " << response;
    } catch (NetworkException &e) {
        qDebug() << "Exception caught in NetworkThread:" << e.message();
        m_jsonLogger->append(serverRequest.toJson());
    }

    emit responseReady(response);
    qInfo(networkThread) << "NetworkWorker->" << response << " [elapsed = " << timer.elapsed() << " ms]";
}

void NetworkWorker::execute(const ServerRequest request)
{
    qCInfo(networkThread) << "NetworkWorker->" << request;
    ServerResponse response(request);

    QElapsedTimer timer;
    timer.start();

    try {
    } catch (NetworkException &e) {
        qDebug() << "Exception caught in NetworkThread:" << e.what();
    }

    emit responseReady(response);
    qInfo(networkThread) << "NetworkWorker->" << response << " [elapsed = " << timer.elapsed() << " ms]";
}

void NetworkWorker::waitForFinished(QNetworkReply *reply)
{
    if (!reply)
        return;

    QEventLoop loop(this);
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
}

void NetworkWorker::flushBackup()
{
    ServerResponse response;
    QNetworkRequest networkRequest;

    try {
        for (const ServerRequest &request : m_jsonLogger->requests()) {
            if (request.queryRequest().type() == QueryRequest::Stock)
                networkRequest.setUrl(QUrl(NetworkThread::STOCK_API_URL));
            else if (request.queryRequest().type() == QueryRequest::Sales)
                networkRequest.setUrl(QUrl(NetworkThread::SALES_API_URL));
            else
                continue;

            networkRequest.setRawHeader("Content-Type", "application/json");
            networkRequest.setRawHeader("Content-Length", QByteArray::number(request.toJson().size()));

            QNetworkReply *networkReply = m_networkManager->post(networkRequest, request.toJson());
            waitForFinished(networkReply);
            if (networkReply->error() != QNetworkReply::NoError)
                throw NetworkException(networkReply->error(),
                                       networkReply->errorString(),
                                       "Failed to receive reply!");
        }
    } catch (NetworkException &e) {
        qDebug() << "Exception caught in NetworkThread:" << e.message();
    }
}

NetworkThread::~NetworkThread()
{
    quit();
    wait();
}

void NetworkThread::run()
{
    exec();
}

void NetworkThread::syncWithServer(const QueryResult result)
{
    if (!result.isSuccessful() || result.request().commandVerb() != QueryRequest::CommandVerb::Read)
        return;

    emit execute(result.request());
}

void NetworkThread::tunnelToServer(const QueryResult result)
{
    if (!result.isSuccessful())
        return;

    emit execute(result.request());
}

NetworkThread::NetworkThread(QObject *parent) :
    QThread(parent)
{
    if (!isRunning()) {
        NetworkWorker *worker = new NetworkWorker;

        connect(worker, &NetworkWorker::responseReady, this, &NetworkThread::responseReady);
        connect(this, QOverload<ServerRequest>::of(&NetworkThread::execute),
                worker, QOverload<ServerRequest>::of(&NetworkWorker::execute));
        connect(this, QOverload<QueryRequest>::of(&NetworkThread::execute),
                worker, QOverload<QueryRequest>::of(&NetworkWorker::execute));
        connect(this, &NetworkThread::finished, worker, &NetworkWorker::deleteLater);

        worker->moveToThread(this);
        start();
    }
}

NetworkThread &NetworkThread::instance()
{
    static NetworkThread instance;
    connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit, &instance, &NetworkThread::quit);
    return instance;
}
