#include "abstractvisuallistmodel.h"
#include "database/databasethread.h"
#include "database/queryexecutor.h"
#include "queryexecutors/sales.h"
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(lcabstractvisuallistmodel, "rrcore.models.abstractvisuallistmodel");

AbstractVisualListModel::AbstractVisualListModel(QObject *parent) :
    AbstractVisualListModel(DatabaseThread::instance(), parent)
{}

AbstractVisualListModel::AbstractVisualListModel(DatabaseThread &thread,
                                                 QObject *parent) :
    QAbstractListModel(parent),
    m_autoQuery(true),
    m_busy(false)
{
    connect(this, &AbstractVisualListModel::execute, &thread, &DatabaseThread::execute);
    connect(&thread, &DatabaseThread::resultReady, this, &AbstractVisualListModel::processResult);

    connect(&thread, &DatabaseThread::resultReady, this, &AbstractVisualListModel::saveRequest);

    connect(this, &AbstractVisualListModel::filterTextChanged, this, &AbstractVisualListModel::filter);
    connect(this, &AbstractVisualListModel::filterColumnChanged, this, &AbstractVisualListModel::filter);
    connect(this, &AbstractVisualListModel::sortOrderChanged, this, &AbstractVisualListModel::filter);
    connect(this, &AbstractVisualListModel::sortColumnChanged, this, &AbstractVisualListModel::filter);
}

AbstractVisualListModel::~AbstractVisualListModel()
{
}

bool AbstractVisualListModel::autoQuery() const
{
    return m_autoQuery;
}

void AbstractVisualListModel::setAutoQuery(bool autoQuery)
{
    if (m_autoQuery == autoQuery)
        return;

    m_autoQuery = autoQuery;

    if (autoQuery)
        tryQuery();

    emit autoQueryChanged();
}

bool AbstractVisualListModel::isBusy() const
{
    return m_busy;
}

QString AbstractVisualListModel::filterText() const
{
    return m_filterCriteria.text;
}

void AbstractVisualListModel::setFilterText(const QString &filterText)
{
    if (m_filterCriteria.text == filterText)
        return;

    m_filterCriteria.text = filterText;
    emit filterTextChanged();
}

int AbstractVisualListModel::filterColumn() const
{
    return m_filterCriteria.columnIndex;
}

QVariant AbstractVisualListModel::get(int row) const
{
    QVariantMap record;
    QHashIterator<int, QByteArray> iter(roleNames());
    while (iter.hasNext()) {
        auto pair = iter.next();

        if (pair.key() >= Qt::UserRole)
            record.insert(pair.value(), data(index(row), pair.key()));
    }

    return QVariant(record);
}

void AbstractVisualListModel::setFilterColumn(int filterColumn)
{
    if (m_filterCriteria.columnIndex == filterColumn)
        return;

    m_filterCriteria.column = columnName(filterColumn);
    m_filterCriteria.columnIndex = filterColumn;
    emit filterColumnChanged();
}

Qt::SortOrder AbstractVisualListModel::sortOrder() const
{
    return m_sortCriteria.order;
}

void AbstractVisualListModel::setSortOrder(Qt::SortOrder sortOrder)
{
    if (m_sortCriteria.order == sortOrder)
        return;

    m_sortCriteria.order = sortOrder;
    emit sortOrderChanged();
}

int AbstractVisualListModel::sortColumn() const
{
    return m_sortCriteria.columnIndex;
}

void AbstractVisualListModel::setSortColumn(int sortColumn)
{
    if (m_sortCriteria.columnIndex == sortColumn)
        return;

    m_sortCriteria.columnIndex = sortColumn;
    emit sortColumnChanged();
}

void AbstractVisualListModel::classBegin()
{

}

void AbstractVisualListModel::componentComplete()
{
    if (m_autoQuery)
        tryQuery();
}

void AbstractVisualListModel::undoLastCommit()
{

}

void AbstractVisualListModel::filter()
{

}

QString AbstractVisualListModel::columnName(int column) const
{
    Q_UNUSED(column)
    return QString();
}

const QueryRequest &AbstractVisualListModel::lastSuccessfulRequest() const
{
    return m_lastSuccessfulRequest;
}

void AbstractVisualListModel::saveRequest(const QueryResult &result)
{
    if (result.isSuccessful() && result.request().receiver() == this
            && result.request().canUndo()
            && !result.request().isUndoSet()) {
        m_lastSuccessfulRequest = result.request();
        QVariantMap params{ result.request().params() };
        params.insert("outcome", result.outcome());
        m_lastSuccessfulRequest.setParams(params);
        qCDebug(lcabstractvisuallistmodel) << "Request saved:" << result.request().command() << this;
    }
}

void AbstractVisualListModel::setBusy(bool busy)
{
    if (m_busy == busy)
        return;

    m_busy = busy;
    emit busyChanged();
}

Utility::FilterCriteria AbstractVisualListModel::filterCriteria() const
{
    return m_filterCriteria;
}

Utility::SortCriteria AbstractVisualListModel::sortCriteria() const
{
    return m_sortCriteria;
}

void AbstractVisualListModel::refresh()
{
    tryQuery();
}
