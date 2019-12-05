#include "abstractvisualtablemodel.h"
#include "database/databasethread.h"
#include "database/queryexecutor.h"

#include <QLoggingCategory>

Q_LOGGING_CATEGORY(abstractVisualTableModel, "rrcore.models.abstractvisualtablemodel");

AbstractVisualTableModel::AbstractVisualTableModel(QObject *parent) :
    AbstractVisualTableModel(DatabaseThread::instance(), parent)
{}

AbstractVisualTableModel::AbstractVisualTableModel(DatabaseThread &thread, QObject *parent) :
    QAbstractTableModel(parent),
    m_autoQuery(true),
    m_busy(false),
    m_filterColumn(-1),
    m_sortOrder(Qt::AscendingOrder),
    m_sortColumn(-1),
    m_tableViewWidth(0.0),
    m_lastQueryExecutor(nullptr)
{
    connect(this, &AbstractVisualTableModel::execute, &thread, &DatabaseThread::execute);
    connect(&thread, &DatabaseThread::resultReady, this, &AbstractVisualTableModel::processResult);

    connect(&thread, &DatabaseThread::execute, this, &AbstractVisualTableModel::cacheQueryExecutor);
    connect(&thread, &DatabaseThread::resultReady, this, &AbstractVisualTableModel::saveRequest);

    connect(this, &AbstractVisualTableModel::filterTextChanged, this, &AbstractVisualTableModel::filter);
    connect(this, &AbstractVisualTableModel::filterColumnChanged, this, &AbstractVisualTableModel::filter);
    connect(this, &AbstractVisualTableModel::sortOrderChanged, this, &AbstractVisualTableModel::filter);
    connect(this, &AbstractVisualTableModel::sortColumnChanged, this, &AbstractVisualTableModel::filter);
}

bool AbstractVisualTableModel::autoQuery() const
{
    return m_autoQuery;
}

void AbstractVisualTableModel::setAutoQuery(bool autoQuery)
{
    if (m_autoQuery == autoQuery)
        return;

    m_autoQuery = autoQuery;

    if (autoQuery)
        tryQuery();

    emit autoQueryChanged();
}

bool AbstractVisualTableModel::isBusy() const
{
    return m_busy;
}

QString AbstractVisualTableModel::filterText() const
{
    return m_filterText;
}

void AbstractVisualTableModel::setFilterText(const QString &filterText)
{
    if (m_filterText == filterText)
        return;

    m_filterText = filterText;
    emit filterTextChanged();
}

int AbstractVisualTableModel::filterColumn() const
{
    return m_filterColumn;
}

QVariant AbstractVisualTableModel::get(int row, int column) const
{
    QVariantMap record;
    QHashIterator<int, QByteArray> iter(roleNames());
    while (iter.hasNext()) {
        auto pair = iter.next();

        if (pair.key() >= Qt::UserRole)
            record.insert(pair.value(), data(index(row, column), pair.key()));
    }

    return QVariant(record);
}

void AbstractVisualTableModel::setFilterColumn(int filterColumn)
{
    if (m_filterColumn == filterColumn)
        return;

    m_filterColumn = filterColumn;
    emit filterColumnChanged();
}

Qt::SortOrder AbstractVisualTableModel::sortOrder() const
{
    return m_sortOrder;
}

void AbstractVisualTableModel::setSortOrder(Qt::SortOrder sortOrder)
{
    if (m_sortOrder == sortOrder)
        return;

    m_sortOrder = sortOrder;
    emit sortOrderChanged();
}

int AbstractVisualTableModel::sortColumn() const
{
    return m_sortColumn;
}

void AbstractVisualTableModel::setSortColumn(int sortColumn)
{
    if (m_sortColumn == sortColumn)
        return;

    m_sortColumn = sortColumn;
    emit sortColumnChanged();
}

qreal AbstractVisualTableModel::tableViewWidth() const
{
    return m_tableViewWidth;
}

void AbstractVisualTableModel::setTableViewWidth(qreal tableViewWidth)
{
    if (m_tableViewWidth > 0.0)
        return;

    m_tableViewWidth = tableViewWidth;
    emit tableViewWidthChanged();
}

void AbstractVisualTableModel::classBegin()
{

}

void AbstractVisualTableModel::componentComplete()
{
    if (m_autoQuery)
        tryQuery();
}

void AbstractVisualTableModel::undoLastCommit()
{
    if (!m_lastQueryExecutor->request().command().isEmpty() && m_lastQueryExecutor->request().receiver()) {
        setBusy(true);
        m_lastQueryExecutor->undoOnNextExecution();
        emit execute(m_lastQueryExecutor);
    } else {
        qCWarning(abstractVisualTableModel) << "AbstractVisualTableModel-> No request to undo.";
    }
}

void AbstractVisualTableModel::filter()
{

}

void AbstractVisualTableModel::saveRequest(const QueryResult &result)
{
    if (!m_lastQueryExecutor)
        return;

    if (result.isSuccessful() && result.request().receiver() == this) {
        if (m_lastQueryExecutor->canUndo() && !m_lastQueryExecutor->isUndoSet() && m_lastQueryExecutor->request() == result.request()) {
            qInfo(abstractVisualTableModel) << "Request saved:" << result.request().command();
            // FIXME: Get rid of this
            QueryRequest &request(m_lastQueryExecutor->request());
            request.params().insert("outcome", result.outcome());

            m_lastQueryExecutor->undoOnNextExecution(false);
        } else {
            m_lastQueryExecutor->deleteLater();
            m_lastQueryExecutor = nullptr;
        }
    }
}

void AbstractVisualTableModel::setBusy(bool busy)
{
    if (m_busy == busy)
        return;

    m_busy = busy;
    emit busyChanged();
}

void AbstractVisualTableModel::cacheQueryExecutor(QueryExecutor *queryExecutor)
{
    m_lastQueryExecutor = queryExecutor;
}

void AbstractVisualTableModel::refresh()
{
    tryQuery();
}

QString AbstractVisualTableModel::columnName(int column) const
{
    Q_UNUSED(column)
    return QString();
}
