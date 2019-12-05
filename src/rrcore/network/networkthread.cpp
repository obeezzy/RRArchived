#include "networkthread.h"
#include "serverresponse.h"
#include "networkexception.h"
#include "requestlogger.h"
#include "user/userprofile.h"

#include <QElapsedTimer>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QEventLoop>
#include "networkurl.h"
#include "networkerror.h"
#include "database/queryexecutor.h"

Q_LOGGING_CATEGORY(networkThread, "rrcore.network.networkthread");

NetworkWorker::NetworkWorker(QObject *parent) :
    QObject(parent)
{
    m_networkManager = new QNetworkAccessManager(this);
    m_requestLogger = new RequestLogger(this);
}

void NetworkWorker::execute(const QueryRequest request)
{
    qCInfo(networkThread) << request;
    ServerResponse response(QueryResult{ request }); // Always know your sender, even if an exception is thrown
    QNetworkRequest networkRequest;
    ServerRequest serverRequest(request);

    QElapsedTimer timer;
    timer.start();

    try {
        networkRequest.setUrl(determineUrl(request));
        networkRequest.setRawHeader("Content-Type", "application/json");
        networkRequest.setRawHeader("Content-Length", QByteArray::number(serverRequest.toJson().size()));
        if (!UserProfile::instance().accessToken().trimmed().isEmpty())
            networkRequest.setRawHeader("Authorization", QByteArray("Bearer ")
                                        .append(UserProfile::instance().accessToken()));

        flushLoggedRequests();

        QNetworkReply *networkReply = m_networkManager->post(networkRequest, serverRequest.toJson());
        waitForFinished(networkReply);

        response = ServerResponse::fromJson(networkReply->readAll(), request);
        QueryResult result{ response.queryResult() };
        response.setQueryResult(result);

        if (networkReply->error() != QNetworkReply::NoError)
            throw NetworkException(response.errorCode(),
                                   response.errorMessage(),
                                   networkReply->error(),
                                   networkReply->errorString());
    } catch (NetworkException &e) {
        response.setErrorCode(e.code());
        response.setErrorMessage(e.message());
        response.setStatusCode(e.statusCode());
        response.setStatusMessage(e.statusMessage());

        if (request.commandVerb() != QueryRequest::CommandVerb::Authenticate
                && request.commandVerb() != QueryRequest::CommandVerb::Read) // Don't store authentication or read commands
            m_requestLogger->push(serverRequest);

        qCWarning(networkThread).nospace() << e;
    }

    emit resultReady(response.queryResult());
    qCInfo(networkThread) << response << " [elapsed = " << timer.elapsed() << " ms]";
}

void NetworkWorker::execute(const ServerRequest request)
{
    qCInfo(networkThread) << request;
    ServerResponse response(request); // Always know the sender
    QNetworkRequest networkRequest;

    QElapsedTimer timer;
    timer.start();

    try {
        networkRequest.setUrl(determineUrl(request));
        networkRequest.setRawHeader("Content-Type", "application/json");
        networkRequest.setRawHeader("Content-Length", QByteArray::number(request.toJson().size()));
        if (!UserProfile::instance().accessToken().trimmed().isEmpty())
            networkRequest.setRawHeader("Authorization", QByteArray("Bearer ")
                                        .append(UserProfile::instance().accessToken()));

        QNetworkReply *networkReply = m_networkManager->post(networkRequest, request.toJson());
        waitForFinished(networkReply);

        response = ServerResponse::fromJson(networkReply->readAll(), request);

        if (networkReply->error() != QNetworkReply::NoError)
            throw NetworkException(response.errorCode(),
                                   response.errorMessage(),
                                   networkReply->error(),
                                   networkReply->errorString());
    } catch (NetworkException &e) {
        response.setErrorCode(e.code());
        response.setErrorMessage(e.message());
        response.setStatusCode(e.statusCode());
        response.setStatusMessage(e.statusMessage());

        qCWarning(networkThread).nospace() << e;
    }

    emit responseReady(response);
    qCInfo(networkThread) << response << " [elapsed = " << timer.elapsed() << " ms]";
}

QUrl NetworkWorker::determineUrl(const QueryRequest &request) const
{
    if (request.queryGroup() == QueryRequest::QueryGroup::Dashboard) {
        return QUrl(NetworkUrl::DASHBOARD_API_URL);
    } else if (request.queryGroup() == QueryRequest::QueryGroup::Stock) {
        return QUrl(NetworkUrl::STOCK_API_URL);
    } else if (request.queryGroup() == QueryRequest::QueryGroup::Sales) {
        return QUrl(NetworkUrl::SALES_API_URL);
    } else if (request.queryGroup() == QueryRequest::QueryGroup::User) {
        if (request.commandVerb() == QueryRequest::CommandVerb::Authenticate) {
            if (request.command() == "sign_in_user")
                return QUrl(NetworkUrl::SIGN_IN_API_URL);
            else if (request.command() == "sign_up_user")
                return QUrl(NetworkUrl::SIGN_UP_API_URL);
            else if (request.command() == "sign_out_user")
                return QUrl(NetworkUrl::SIGN_OUT_API_URL);
            else if (request.command() == "change_password")
                return QUrl(NetworkUrl::CHANGE_PASSWORD_API_URL);
        } else {
            return QUrl(NetworkUrl::USER_API_URL);
        }
    }

    throw NetworkException(NetworkError::ServerErrorCode::UnableToDetermineDestinationUrl,
                           QStringLiteral("Unable to determine destination URL for command '%1'").arg(request.command()));
}

QUrl NetworkWorker::determineUrl(const ServerRequest &request) const
{
    if (request.action() == "link_account")
        return QUrl(NetworkUrl::LINK_ACCOUNT_URL);
    else if (request.action() == "link_business_store")
        return QUrl(NetworkUrl::LINK_BUSINESS_STORE_URL);

    throw NetworkException(NetworkError::ServerErrorCode::UnableToDetermineDestinationUrl,
                           QStringLiteral("Unable to determine destination URL for action '%1'").arg(request.action()));
}

void NetworkWorker::waitForFinished(QNetworkReply *reply)
{
    if (!reply)
        return;

    QEventLoop loop(this);
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    qCDebug(networkThread) << "NetworkWorker-> Reply received for" << reply->request().url();
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
            if (!UserProfile::instance().accessToken().trimmed().isEmpty())
                networkRequest.setRawHeader("Authorization", QByteArray("Bearer ")
                                            .append(UserProfile::instance().accessToken()));

            QNetworkReply *networkReply = m_networkManager->post(networkRequest, request.toJson());
            waitForFinished(networkReply);

            response = ServerResponse::fromJson(networkReply->readAll(), request);

            if (networkReply->error() != QNetworkReply::NoError)
                throw NetworkException(response.errorCode(),
                                       response.errorMessage(),
                                       networkReply->error(),
                                       networkReply->errorString());

            m_requestLogger->pop();
        }
    } catch (NetworkException &e) {
        qCWarning(networkThread) << "Exception caught while flushing backup:" << e.message() << e.statusMessage();
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

void NetworkThread::tunnelToServer(QueryExecutor *queryExecutor)
{
    emit execute(queryExecutor->request());
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
