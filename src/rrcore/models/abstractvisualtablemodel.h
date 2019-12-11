#ifndef ABSTRACTVISUALTABLEMODEL_H
#define ABSTRACTVISUALTABLEMODEL_H

#include <QObject>
#include <QAbstractTableModel>
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

class AbstractVisualTableModel : public QAbstractTableModel, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(bool autoQuery READ autoQuery WRITE setAutoQuery NOTIFY autoQueryChanged)
    Q_PROPERTY(bool busy READ isBusy NOTIFY busyChanged)
    Q_PROPERTY(QString filterText READ filterText WRITE setFilterText NOTIFY filterTextChanged)
    Q_PROPERTY(int filterColumn READ filterColumn WRITE setFilterColumn NOTIFY filterColumnChanged)
    Q_PROPERTY(Qt::SortOrder sortOrder READ sortOrder WRITE setSortOrder NOTIFY sortOrderChanged)
    Q_PROPERTY(int sortColumn READ sortColumn WRITE setSortColumn NOTIFY sortColumnChanged)
    Q_PROPERTY(qreal tableViewWidth READ tableViewWidth WRITE setTableViewWidth) // NOTE: Can only be set once!
public:
    explicit AbstractVisualTableModel(QObject *parent = nullptr);
    explicit AbstractVisualTableModel(DatabaseThread &thread, QObject *parent = nullptr);
    virtual ~AbstractVisualTableModel() override = default;

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

    qreal tableViewWidth() const;
    void setTableViewWidth(qreal tableViewWidth);

    Q_INVOKABLE QVariant get(int row, int column) const;

    void classBegin() override;
    void componentComplete() override;
public slots:
    virtual void undoLastCommit();
    virtual void refresh();
protected:
    virtual void tryQuery() = 0;
    virtual void processResult(const QueryResult result) = 0;
    virtual QString columnName(int column) const;
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

    void tableViewWidthChanged();

    void success(int successCode = -1);
    void error(int errorCode = -1);
private:
    bool m_autoQuery;
    bool m_busy;
    QString m_filterText;
    int m_filterColumn;
    Qt::SortOrder m_sortOrder;
    int m_sortColumn;
    qreal m_tableViewWidth;
    QueryRequest m_lastSuccessfulRequest;

    void saveRequest(const QueryResult &result);
};

Q_DECLARE_LOGGING_CATEGORY(abstractVisualTableModel);

#endif // ABSTRACTVISUALTABLEMODEL_H
