#include "abstractpusher.h"

#include "database/databasethread.h"
#include "database/queryrequest.h"
#include "database/queryresult.h"

AbstractPusher::AbstractPusher(QObject *parent) :
    AbstractPusher(DatabaseThread::instance(), parent)
{}

AbstractPusher::AbstractPusher(DatabaseThread &thread, QObject *parent) :
    QObject(parent),
    m_busy(false)
{
    connect(this, &AbstractPusher::executeRequest, &thread, &DatabaseThread::execute);
    connect(&thread, &DatabaseThread::resultReady, this, &AbstractPusher::processResult);

    connect(&thread, &DatabaseThread::resultReady, this, &AbstractPusher::saveRequest);
}

AbstractPusher::~AbstractPusher()
{

}

bool AbstractPusher::isBusy() const
{
    return m_busy;
}

void AbstractPusher::setBusy(bool busy)
{
    if (m_busy == busy)
        return;

    m_busy = busy;
    emit busyChanged();
}

void AbstractPusher::undoLastCommit()
{
    setBusy(true);
    QueryRequest request(m_lastRequest);

    request.setCommand(QString("undo_") + request.command(), request.params(), request.type());
    emit executeRequest(request);
}

void AbstractPusher::saveRequest(const QueryResult &result)
{
    if (result.isSuccessful() && result.request().receiver() == this) {
        if (result.request().params().value("can_undo").toBool() && !result.request().command().startsWith("undo_")) {
            QueryRequest request(result.request());

            QVariantMap params = request.params();
            params.insert("outcome", result.outcome());
            params.remove("can_undo");

            request.setCommand(request.command(), params, request.type());
            m_lastRequest = request;
        } else {
            m_lastRequest = QueryRequest();
        }
    }
}

void AbstractPusher::setLastRequest(const QueryRequest &lastRequest)
{
    m_lastRequest = lastRequest;
}

QueryRequest AbstractPusher::lastRequest() const
{
    return m_lastRequest;
}
