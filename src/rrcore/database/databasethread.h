#ifndef DATABASETHREAD_H
#define DATABASETHREAD_H

#include <QThread>
#include <QSqlDatabase>
#include "queryrequest.h"
#include "queryresult.h"

class DatabaseThread : public QThread
{
    Q_OBJECT
public:
    static DatabaseThread &instance();
    ~DatabaseThread();

    void run() override final;
signals:
    void resultsReady(const QueryResult &);
public slots:
    void execute(const QueryRequest &);
private:
    QSqlDatabase m_connection;
    static DatabaseThread *m_instance;

    explicit DatabaseThread(QObject *parent = nullptr);
};

#endif // DATABASETHREAD_H
