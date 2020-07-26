#ifndef DATABASETHREAD_H
#define DATABASETHREAD_H

#include <QLoggingCategory>
#include <QSqlDatabase>
#include <QThread>
#include "queryrequest.h"
#include "queryresult.h"

class QueryExecutor;

class DatabaseWorker : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseWorker(QObject* parent = nullptr);
    ~DatabaseWorker();

    void execute(QueryExecutor* queryExecutor);
signals:
    void resultReady(const QueryResult result);
};

class DatabaseThread : public QThread
{
    Q_OBJECT
public:
    explicit DatabaseThread(QueryResult*,
                            QObject* parent = nullptr);  // For testing

    static DatabaseThread& instance();
    virtual ~DatabaseThread() override;

    DatabaseThread(DatabaseThread const&) = delete;
    void operator=(DatabaseThread const&) = delete;

    void run() override final;
signals:
    void execute(QueryExecutor* queryExecutor);
    void resultReady(const QueryResult result);

private:
    explicit DatabaseThread(QObject* parent = nullptr);
};

Q_DECLARE_LOGGING_CATEGORY(lcdatabasethread);

#endif  // DATABASETHREAD_H
