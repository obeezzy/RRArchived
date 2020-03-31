#include "mockdatabasethread.h"
#include "database/queryresult.h"
#include "database/queryexecutor.h"

MockDatabaseThread::MockDatabaseThread(QueryResult *result,
                                       QObject *parent) :
    DatabaseThread(result, parent),
    m_result(result)
{
    connect(this, &MockDatabaseThread::execute, this, &MockDatabaseThread::emitResult);
}

MockDatabaseThread::~MockDatabaseThread()
{
    *m_result = QueryResult();
}

void MockDatabaseThread::emitResult(QueryExecutor *queryExecutor)
{
    m_result->setRequest(queryExecutor->request());
    emit resultReady(*m_result);
}
