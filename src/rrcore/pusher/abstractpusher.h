#ifndef ABSTRACTPUSHER_H
#define ABSTRACTPUSHER_H

#include <QObject>
#include <QSharedPointer>
#include "database/queryrequest.h"
#include "database/queryresult.h"

class DatabaseThread;
class QueryResult;
class QueryExecutor;

class AbstractPusher : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool busy READ isBusy NOTIFY busyChanged)
public:
    explicit AbstractPusher(QObject *parent = nullptr);
    explicit AbstractPusher(DatabaseThread &thread, QObject *parent = nullptr);
    virtual ~AbstractPusher();

    bool isBusy() const;
protected:
    void setBusy(bool);
    virtual void processResult(const QueryResult result) = 0;

    QueryExecutor *lastQueryExecutor() const;
signals:
    void busyChanged();
    void success(int successCode = 0);
    void error(int errorCode = 0);
    void execute(QueryExecutor *);
public slots:
    virtual void push() = 0;
    virtual void undoLastCommit();
private:
    bool m_busy;
    QSharedPointer<QueryExecutor> m_lastQueryExecutor;

    void cacheQueryExecutor(QueryExecutor *);
    void saveRequest(const QueryResult &result);
};

#endif // ABSTRACTPUSHER_H
