#ifndef DATABASETHREAD_H
#define DATABASETHREAD_H

#include <QThread>
#include <QSqlDatabase>
#include "queryrequest.h"
#include "queryresult.h"

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = nullptr);
    ~Worker();

    void execute(const QueryRequest request);
signals:
    void resultReady(const QueryResult result);
private:
    QSqlDatabase m_connection;

    void connectToTestDatabase();
};

class DatabaseThread : public QThread
{
    Q_OBJECT
public:
    static DatabaseThread &instance();
    ~DatabaseThread() override;

    DatabaseThread(DatabaseThread const &) = delete;
    void operator=(DatabaseThread const &) = delete;

    void run() override final;
signals:
    void execute(const QueryRequest request);
    void resultReady(const QueryResult result);
private:
    explicit DatabaseThread(QObject *parent = nullptr);
};

#endif // DATABASETHREAD_H
