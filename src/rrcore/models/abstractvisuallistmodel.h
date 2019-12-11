#ifndef ABSTRACTVISUALLISTMODEL_H
#define ABSTRACTVISUALLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QQmlParserStatus>
#include <QVariant>
#include <QLoggingCategory>
#include "database/queryrequest.h"
#include "database/queryresult.h"
#include "database/queryexecutor.h"

class QueryRequest;
class QueryResult;
class DatabaseThread;
class QueryExecutor;

class AbstractVisualListModel : public QAbstractListModel, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(bool autoQuery READ autoQuery WRITE setAutoQuery NOTIFY autoQueryChanged)
    Q_PROPERTY(bool busy READ isBusy NOTIFY busyChanged)
    Q_PROPERTY(QString filterText READ filterText WRITE setFilterText NOTIFY filterTextChanged)
    Q_PROPERTY(int filterColumn READ filterColumn WRITE setFilterColumn NOTIFY filterColumnChanged)
    Q_PROPERTY(Qt::SortOrder sortOrder READ sortOrder WRITE setSortOrder NOTIFY sortOrderChanged)
    Q_PROPERTY(int sortColumn READ sortColumn WRITE setSortColumn NOTIFY sortColumnChanged)
public:
    explicit AbstractVisualListModel(QObject *parent = nullptr);
    explicit AbstractVisualListModel(DatabaseThread &thread, QObject *parent = nullptr);
    virtual ~AbstractVisualListModel() override;

    bool autoQuery() const;
    void setAutoQuery(bool);

    bool isBusy() const;

    QString filterText() const;
    void setFilterText(const QString &filterText);

    int filterColumn() const;
    void setFilterColumn(int filterColumn);

    Qt::SortOrder sortOrder() const;
    void setSortOrder(Qt::SortOrder sortOrder);

    int sortColumn() const;
    void setSortColumn(int sortColumn);

    Q_INVOKABLE QVariant get(int row) const;

    void classBegin() override;
    void componentComplete() override;
public slots:
    virtual void undoLastCommit();
    virtual void refresh();
protected:
    virtual void tryQuery() = 0;
    virtual void processResult(const QueryResult result) = 0;
    virtual void filter();
    void setBusy(bool);
    const QueryRequest &lastSuccessfulRequest() const;
signals:
    void execute(QueryExecutor *);
    void autoQueryChanged();
    void busyChanged();

    void filterTextChanged();
    void filterColumnChanged();

    void sortOrderChanged();
    void sortColumnChanged();

    void success(int successCode = -1);
    void error(int errorCode = -1);
private:
    bool m_autoQuery;
    bool m_busy;
    QString m_filterText;
    int m_filterColumn;
    Qt::SortOrder m_sortOrder;
    int m_sortColumn;
    QueryRequest m_lastSuccessfulRequest;

    void saveRequest(const QueryResult &result);
};

Q_DECLARE_LOGGING_CATEGORY(abstractVisualListModel);

#endif // ABSTRACTVISUALLISTMODEL_H
