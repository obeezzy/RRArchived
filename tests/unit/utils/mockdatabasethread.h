#ifndef MOCKDATABASETHREAD_H
#define MOCKDATABASETHREAD_H

#include "database/databasethread.h"
#include "database/queryresult.h"

class QueryExecutor;

class MockDatabaseThread : public DatabaseThread
{
    Q_OBJECT
public:
    explicit MockDatabaseThread(QueryResult *result);
private:
    QueryResult *m_result;

    void emitResult(QueryExecutor *queryExecutor);
};

#endif // MOCKDATABASETHREAD_H
