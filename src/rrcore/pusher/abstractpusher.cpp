#include "abstractpusher.h"

#include "database/databasethread.h"
#include "database/queryrequest.h"
#include "database/queryresult.h"
#include "database/queryexecutor.h"

#include <QLoggingCategory>

Q_LOGGING_CATEGORY(abstractPusher, "rrcore.database.abstractpusher");

AbstractPusher::AbstractPusher(QObject *parent) :
    AbstractPusher(DatabaseThread::instance(), parent)
{}

AbstractPusher::AbstractPusher(DatabaseThread &thread, QObject *parent) :
    QObject(parent),
    m_busy(false),
    m_lastQueryExecutor(nullptr)
{
    connect(this, &AbstractPusher::execute, &thread, &DatabaseThread::execute);

    connect(&thread, &DatabaseThread::execute, this, &AbstractPusher::cacheQueryExecutor);
    connect(&thread, &DatabaseThread::resultReady, this, &AbstractPusher::processResult);

    connect(&thread, &DatabaseThread::resultReady, this, &AbstractPusher::saveRequest);
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
    m_lastQueryExecutor.undoOnNextExecution();
    emit execute(new QueryExecutor(m_lastQueryExecutor));
}

void AbstractPusher::saveRequest(const QueryResult &result)
{
    if (!m_lastQueryExecutor.isValid())
        return;

    if (result.isSuccessful() && result.request().receiver() == this && result.request() == m_lastQueryExecutor.request()) {
        if (m_lastQueryExecutor.canUndo() && !m_lastQueryExecutor.isUndoSet()) {
            qCDebug(abstractPusher) << "Request saved:" << result.request();
            // FIXME: Remove this!
            QueryRequest &request(m_lastQueryExecutor.request());
            request.params().insert("outcome", result.outcome());

            m_lastQueryExecutor.undoOnNextExecution(false);
        } else {
            m_lastQueryExecutor = QueryExecutor();
        }
    }
}

void AbstractPusher::cacheQueryExecutor(QueryExecutor *queryExecutor)
{
    if (queryExecutor->canUndo())
        m_lastQueryExecutor = *queryExecutor;
}
