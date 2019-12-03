#include "mockdatabasethread.h"
#include "database/queryresult.h"
#include "database/queryexecutor.h"

MockDatabaseThread::MockDatabaseThread(QueryResult *result) :
    DatabaseThread(result),
    m_result(result)
{
    connect(this, &MockDatabaseThread::execute, this, &MockDatabaseThread::emitResult);
}

void MockDatabaseThread::emitResult(QueryExecutor *queryExecutor)
{
    m_result->setRequest(queryExecutor->request());
    emit resultReady(*m_result);
}
