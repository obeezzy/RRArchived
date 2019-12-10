#ifndef ABSTRACTPUSHER_H
#define ABSTRACTPUSHER_H

#include <QObject>
#include <QLoggingCategory>
#include "database/queryrequest.h"
#include "database/queryresult.h"
#include "database/queryexecutor.h"

class DatabaseThread;
class QueryResult;

class AbstractPusher : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool busy READ isBusy NOTIFY busyChanged)
public:
    explicit AbstractPusher(QObject *parent = nullptr);
    explicit AbstractPusher(DatabaseThread &thread, QObject *parent = nullptr);
    virtual ~AbstractPusher() = default;

    bool isBusy() const;
protected:
    void setBusy(bool);
    virtual void processResult(const QueryResult result) = 0;
    const QueryRequest &lastSuccessfulRequest() const;
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
    QueryRequest m_lastSuccessfulRequest;

    void saveRequest(const QueryResult &result);
};

Q_DECLARE_LOGGING_CATEGORY(abstractPusher);

#endif // ABSTRACTPUSHER_H
