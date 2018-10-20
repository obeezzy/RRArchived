#include "mockdatabasethread.h"
#include "database/queryresult.h"

MockDatabaseThread::MockDatabaseThread(QueryResult *result) :
    DatabaseThread(result),
    m_result(result)
{
    connect(this, &MockDatabaseThread::execute, this, &MockDatabaseThread::emitResult);
}

void MockDatabaseThread::emitResult(const QueryRequest &request)
{
    m_result->setRequest(request);
    emit resultReady(*m_result);
}
