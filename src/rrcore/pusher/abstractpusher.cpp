#include "abstractpusher.h"
#include <QLoggingCategory>
#include "database/databasethread.h"
#include "database/queryexecutor.h"
#include "database/queryrequest.h"
#include "database/queryresult.h"

Q_LOGGING_CATEGORY(abstractPusher, "rrcore.database.abstractpusher");

AbstractPusher::AbstractPusher(QObject* parent)
    : AbstractPusher(DatabaseThread::instance(), parent)
{}

AbstractPusher::AbstractPusher(DatabaseThread& thread, QObject* parent)
    : QObject(parent),
      m_busy(false)
{
    connect(this, &AbstractPusher::execute, &thread, &DatabaseThread::execute);

    connect(&thread, &DatabaseThread::resultReady, this,
            &AbstractPusher::processResult);
    connect(&thread, &DatabaseThread::resultReady, this,
            &AbstractPusher::saveRequest);
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

const QueryRequest& AbstractPusher::lastSuccessfulRequest() const
{
    return m_lastSuccessfulRequest;
}

void AbstractPusher::undoLastCommit() {}

void AbstractPusher::saveRequest(const QueryResult& result)
{
    if (result.isSuccessful() && result.request().receiver() == this &&
        result.request().canUndo() && !result.request().isUndoSet()) {
        m_lastSuccessfulRequest = result.request();
        QVariantMap params{result.request().params()};
        params.insert("outcome", result.outcome());
        m_lastSuccessfulRequest.setParams(params);
        qCDebug(abstractPusher)
            << "Request saved:" << result.request().command() << this;
    }
}

void AbstractPusher::validateResult(const QueryResult result)
{
    if (result.request().receiver() != this)
        return;

    if (canProcessResult(result))
        processResult(result);
    else
        qFatal(
            "Validation failed for %s.\nActual returned keys: %s.\nExpected "
            "returned keys: %s",
            result.request().command().toStdString().c_str(),
            result.outcome().toMap().keys().join(", ").toStdString().c_str(),
            result.errorDetails().toStdString().c_str());
}
