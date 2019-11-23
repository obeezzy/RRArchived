#include "networkthread.h"
#include "serverresponse.h"
#include "networkexception.h"
#include "requestlogger.h"

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
    m_requestLogger = new RequestLogger(this);
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
        networkRequest.setUrl(determineUrl(request));
        networkRequest.setRawHeader("Content-Type", "application/json");
        networkRequest.setRawHeader("Content-Length", QByteArray::number(serverRequest.toJson().size()));
        networkRequest.setRawHeader("x-access-token", QSettings().value("access_token").toByteArray());            

        flushLoggedRequests();

        QNetworkReply *networkReply = m_networkManager->post(networkRequest, serverRequest.toJson());
        waitForFinished(networkReply);

        response = ServerResponse::fromJson(networkReply->readAll());
        QueryResult result{ response.queryResult() };
        result.setRequest(request);
        response.setQueryResult(result);

        if (networkReply->error() != QNetworkReply::NoError)
            throw NetworkException(networkReply->error(),
                                   networkReply->errorString(),
                                   response.errorMessage());

        if (!response.isSuccessful())
            throw NetworkException(RequestFailed,
                                   response.errorMessage(),
                                   "Request failed.");

        qInfo() << "Response? " << response;
    } catch (NetworkException &e) {
        qWarning() << "Exception caught in NetworkThread:" << e.message() << e.userMessage();
        if (request.commandVerb() != QueryRequest::CommandVerb::Authenticate
                && request.commandVerb() != QueryRequest::CommandVerb::Read) // Don't store authentication or read commands
            m_requestLogger->push(serverRequest);
    }

    emit resultReady(response.queryResult());
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

QUrl NetworkWorker::determineUrl(const QueryRequest &request) const
{
    if (request.type() == QueryRequest::Dashboard) {
        return QUrl(NetworkThread::DASHBOARD_API_URL);
    } else if (request.type() == QueryRequest::Stock) {
        return QUrl(NetworkThread::STOCK_API_URL);
    } else if (request.type() == QueryRequest::Sales) {
        return QUrl(NetworkThread::SALES_API_URL);
    } else if (request.type() == QueryRequest::User) {
        if (request.commandVerb() == QueryRequest::CommandVerb::Authenticate) {
            if (request.command() == "sign_in_user")
                return QUrl(NetworkThread::SIGN_IN_API_URL);
            else if (request.command() == "sign_up_user")
                return QUrl(NetworkThread::SIGN_UP_API_URL);
            else if (request.command() == "sign_out_user")
                return QUrl(NetworkThread::SIGN_OUT_API_URL);
        } else {
            return QUrl(NetworkThread::USER_API_URL);
        }
    }

    throw NetworkException(UnableToDetermineDestinationUrl,
                           QStringLiteral("Unable to determine destination URL for command '%1'").arg(request.command()),
                           QStringLiteral("Unable to determine destination URL for command '%1'").arg(request.command())
                           );
}

void NetworkWorker::waitForFinished(QNetworkReply *reply)
{
    if (!reply)
        return;

    QEventLoop loop(this);
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    qInfo(networkThread) << "NetworkWorker-> Reply received for" << reply->request().url();
}

void NetworkWorker::flushLoggedRequests()
{
    ServerResponse response;
    QNetworkRequest networkRequest;

    try {
        while (m_requestLogger->hasNext()) {
            const ServerRequest &request = m_requestLogger->nextRequest();

            networkRequest.setUrl(determineUrl(request.queryRequest()));
            networkRequest.setRawHeader("Content-Type", "application/json");
            networkRequest.setRawHeader("Content-Length", QByteArray::number(request.toJson().size()));

            QNetworkReply *networkReply = m_networkManager->post(networkRequest, request.toJson());
            waitForFinished(networkReply);
            if (networkReply->error() != QNetworkReply::NoError)
                throw NetworkException(networkReply->error(),
                                       networkReply->errorString(),
                                       "Failed to receive reply!");

            m_requestLogger->pop();
        }
    } catch (NetworkException &e) {
        qWarning(networkThread) << "Exception caught while flushing backup:" << e.message();
        throw;
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
    if (!result.isSuccessful() || result.request().commandVerb() == QueryRequest::CommandVerb::Read)
        return;

    emit execute(result.request());
}

void NetworkThread::tunnelToServer(const QueryRequest request)
{
    emit execute(request);
}

NetworkThread::NetworkThread(QObject *parent) :
    QThread(parent)
{
    if (!isRunning()) {
        NetworkWorker *worker = new NetworkWorker;

        connect(worker, &NetworkWorker::responseReady, this, &NetworkThread::responseReady);
        connect(worker, &NetworkWorker::resultReady, this, &NetworkThread::resultReady);
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
    connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit,
            &instance, &NetworkThread::quit);
    return instance;
}
