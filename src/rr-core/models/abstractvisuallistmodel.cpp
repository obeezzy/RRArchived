#include "abstractvisuallistmodel.h"
#include "rr-core/database/databasethread.h"

AbstractVisualListModel::AbstractVisualListModel(QObject *parent)
    : m_autoQuery(true)
    , m_busy(false)
    , m_filterColumn(-1)
    , QAbstractListModel(parent)
{
    connect(this, &AbstractVisualListModel::executeRequest, &DatabaseThread::instance(), &DatabaseThread::execute);
    connect(&DatabaseThread::instance(), &DatabaseThread::resultsReady, this, &AbstractVisualListModel::processResult);

    connect(this, &AbstractVisualListModel::filterTextChanged, this, &AbstractVisualListModel::filter);
    connect(this, &AbstractVisualListModel::filterColumnChanged, this, &AbstractVisualListModel::filter);
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
    return m_filterText;
}

void AbstractVisualListModel::setFilterText(const QString &filterText)
{
    if (m_filterText == filterText)
        return;

    m_filterText = filterText;
    emit filterTextChanged();
}

int AbstractVisualListModel::filterColumn() const
{
    return m_filterColumn;
}

QVariant AbstractVisualListModel::get(int row) const
{
    return data(index(row));
}

void AbstractVisualListModel::setFilterColumn(int filterColumn)
{
    if (m_filterColumn == filterColumn)
        return;

    m_filterColumn = filterColumn;
    emit filterColumnChanged();
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

void AbstractVisualListModel::setBusy(bool busy)
{
    if (m_busy == busy)
        return;

    m_busy = busy;
    emit busyChanged();
}
