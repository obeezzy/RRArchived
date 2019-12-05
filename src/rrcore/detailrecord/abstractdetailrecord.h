#ifndef ABSTRACTDETAILRECORD_H
#define ABSTRACTDETAILRECORD_H

#include <QObject>
#include <QQmlParserStatus>
#include "database/queryrequest.h"
#include "database/queryresult.h"

class DatabaseThread;
class QueryExecutor;

class AbstractDetailRecord : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(bool autoQuery READ autoQuery WRITE setAutoQuery NOTIFY autoQueryChanged)
    Q_PROPERTY(bool busy READ isBusy NOTIFY busyChanged)
public:
    explicit AbstractDetailRecord(QObject *parent = nullptr);
    explicit AbstractDetailRecord(DatabaseThread &thread, QObject *parent = nullptr); // For testing
    virtual ~AbstractDetailRecord() override;

    bool autoQuery() const;
    void setAutoQuery(bool autoQuery);

    bool isBusy() const;

    void classBegin() override final;
    void componentComplete() override final;
public slots:
    virtual void refresh();
protected:
    virtual void tryQuery() = 0;
    virtual void processResult(const QueryResult result) = 0;
    void setBusy(bool busy);
signals:
    void execute(QueryExecutor *);
    void autoQueryChanged();
    void busyChanged();

    void success(int successCode = -1);
    void error(int errorCode = -1);
private:
    bool m_autoQuery;
    bool m_busy;
};

#endif // ABSTRACTDETAILRECORD_H
