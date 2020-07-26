#include "mockdatabasethread.h"
#include "database/queryexecutor.h"
#include "database/queryresult.h"

MockDatabaseThread::MockDatabaseThread(QObject* parent)
    : DatabaseThread(&m_result, parent)
{
    connect(this, &MockDatabaseThread::execute, this,
            &MockDatabaseThread::emitResult);
}

QueryResult& MockDatabaseThread::result()
{
    return m_result;
}

void MockDatabaseThread::emitResult(QueryExecutor* queryExecutor)
{
    m_result.setRequest(queryExecutor->request());
    emit resultReady(m_result);
}
