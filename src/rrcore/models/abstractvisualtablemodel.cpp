#include "abstractvisualtablemodel.h"
#include "database/databasethread.h"
#include "database/queryexecutor.h"
#include "utility/common/filtercriteria.h"
#include "utility/common/sortcriteria.h"
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(lcabstractvisualtablemodel, "rrcore.models.abstractvisualtablemodel");

AbstractVisualTableModel::AbstractVisualTableModel(QObject *parent) :
    AbstractVisualTableModel(DatabaseThread::instance(), parent)
{}

AbstractVisualTableModel::AbstractVisualTableModel(DatabaseThread &thread, QObject *parent) :
    QAbstractTableModel(parent)
{
    connect(this, &AbstractVisualTableModel::execute, &thread, &DatabaseThread::execute);
    connect(&thread, &DatabaseThread::resultReady, this, &AbstractVisualTableModel::processResult);

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
    return m_filterCriteria.text;
}

void AbstractVisualTableModel::setFilterText(const QString &filterText)
{
    if (m_filterCriteria.text == filterText)
        return;

    m_filterCriteria.text = filterText;
    emit filterTextChanged();
}

int AbstractVisualTableModel::filterColumn() const
{
    return m_filterCriteria.columnIndex;
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
    if (m_filterCriteria.columnIndex == filterColumn)
        return;

    m_filterCriteria.columnIndex = filterColumn;
    m_filterCriteria.column = columnName(filterColumn);
    emit filterColumnChanged();
}

Qt::SortOrder AbstractVisualTableModel::sortOrder() const
{
    return m_sortCriteria.order;
}

void AbstractVisualTableModel::setSortOrder(Qt::SortOrder sortOrder)
{
    if (m_sortCriteria.order == sortOrder)
        return;

    m_sortCriteria.order = sortOrder;
    emit sortOrderChanged();
}

int AbstractVisualTableModel::sortColumn() const
{
    return m_sortCriteria.columnIndex;
}

void AbstractVisualTableModel::setSortColumn(int sortColumn)
{
    if (m_sortCriteria.columnIndex == sortColumn)
        return;

    m_sortCriteria.columnIndex = sortColumn;
    m_sortCriteria.column = columnName(sortColumn);
    emit sortColumnChanged();
}

double AbstractVisualTableModel::tableViewWidth() const
{
    return m_tableViewWidth;
}

void AbstractVisualTableModel::setTableViewWidth(double tableViewWidth)
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
}

void AbstractVisualTableModel::filter()
{

}

void AbstractVisualTableModel::saveRequest(const QueryResult &result)
{
    if (result.isSuccessful() && result.request().receiver() == this
            && result.request().canUndo()
            && !result.request().isUndoSet()) {
        m_lastSuccessfulRequest = result.request();
        QVariantMap params{ result.request().params() };
        params.insert("outcome", result.outcome());
        m_lastSuccessfulRequest.setParams(params);
        qCDebug(lcabstractvisualtablemodel) << "Request saved:" << result.request().command() << this;
    }
}

void AbstractVisualTableModel::setBusy(bool busy)
{
    if (m_busy == busy)
        return;

    m_busy = busy;
    emit busyChanged();
}

const QueryRequest &AbstractVisualTableModel::lastSuccessfulRequest() const
{
    return m_lastSuccessfulRequest;
}

Utility::FilterCriteria AbstractVisualTableModel::filterCriteria() const
{
    return m_filterCriteria;
}

Utility::SortCriteria AbstractVisualTableModel::sortCriteria() const
{
    return m_sortCriteria;
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
