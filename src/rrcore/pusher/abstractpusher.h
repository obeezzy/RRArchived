#ifndef ABSTRACTPUSHER_H
#define ABSTRACTPUSHER_H

#include <QObject>
#include "database/queryrequest.h"
#include "database/queryresult.h"

class QueryResult;

class AbstractPusher : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool busy READ isBusy NOTIFY busyChanged)
public:
    explicit AbstractPusher(QObject *parent = nullptr);
    virtual ~AbstractPusher();

    bool isBusy() const;
protected:
    void setBusy(bool);
    virtual void processResult(const QueryResult result) = 0;

    void setLastRequest(const QueryRequest &lastRequest);
    QueryRequest lastRequest() const;
signals:
    void busyChanged();
    void success(int successCode = 0);
    void error(int errorCode = 0);
    void executeRequest(const QueryRequest request);
public slots:
    virtual void push() = 0;
    virtual void undoLastCommit();
private:
    bool m_busy;
    QueryRequest m_lastRequest;

    void saveRequest(const QueryResult &result);
};

#endif // ABSTRACTPUSHER_H
