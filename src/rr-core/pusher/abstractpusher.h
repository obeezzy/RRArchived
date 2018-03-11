#ifndef ABSTRACTPUSHER_H
#define ABSTRACTPUSHER_H

#include <QObject>

class QueryRequest;
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
    virtual void processResult(const QueryResult &result) = 0;
signals:
    void busyChanged();
    void success();
    void error(int errorCode);
    void executeRequest(const QueryRequest &);
public slots:
    virtual void push() = 0;
private:
    bool m_busy;
};

#endif // ABSTRACTPUSHER_H
