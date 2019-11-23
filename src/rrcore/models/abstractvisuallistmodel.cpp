#include "abstractvisuallistmodel.h"
#include "database/databasethread.h"

AbstractVisualListModel::AbstractVisualListModel(QObject *parent) :
    AbstractVisualListModel(DatabaseThread::instance(), parent)
{}

AbstractVisualListModel::AbstractVisualListModel(DatabaseThread &thread, QObject *parent) :
    QAbstractListModel(parent),
    m_autoQuery(true),
    m_busy(false),
    m_filterColumn(-1),
    m_sortOrder(Qt::AscendingOrder),
    m_sortColumn(-1)
{
    connect(this, &AbstractVisualListModel::executeRequest, &thread, &DatabaseThread::execute);
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
    if (m_filterColumn == filterColumn)
        return;

    m_filterColumn = filterColumn;
    emit filterColumnChanged();
}

Qt::SortOrder AbstractVisualListModel::sortOrder() const
{
    return m_sortOrder;
}

void AbstractVisualListModel::setSortOrder(Qt::SortOrder sortOrder)
{
    if (m_sortOrder == sortOrder)
        return;

    m_sortOrder = sortOrder;
    emit sortOrderChanged();
}

int AbstractVisualListModel::sortColumn() const
{
    return m_sortColumn;
}

void AbstractVisualListModel::setSortColumn(int sortColumn)
{
    if (m_sortColumn == sortColumn)
        return;

    m_sortColumn = sortColumn;
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
    if (!m_lastRequest.command().isEmpty() && m_lastRequest.receiver()) {
        setBusy(true);
        QueryRequest request(m_lastRequest);

        request.setCommand(QString("undo_") + request.command(), request.params(), request.type());
        emit executeRequest(request);
    } else {
        qWarning() << "AbstractVisualListModel-> No request to undo.";
    }
}

void AbstractVisualListModel::filter()
{

}

void AbstractVisualListModel::saveRequest(const QueryResult &result)
{
    if (result.isSuccessful() && result.request().receiver() == this) {
        if (result.request().params().value("can_undo").toBool() && !result.request().command().startsWith("undo_")) {
            qInfo() << "Request saved:" << result.request().command();
            QueryRequest request(result.request());

            QVariantMap params = request.params();
            params.insert("outcome", result.outcome());
            params.remove("can_undo");

            request.setCommand(request.command(), params, request.type());
            m_lastRequest = request;
        } else {
            m_lastRequest = QueryRequest();
        }
    }
}

void AbstractVisualListModel::setBusy(bool busy)
{
    if (m_busy == busy)
        return;

    m_busy = busy;
    emit busyChanged();
}

void AbstractVisualListModel::setLastRequest(const QueryRequest &lastRequest)
{
    m_lastRequest = lastRequest;
}

QueryRequest AbstractVisualListModel::lastRequest() const
{
    return m_lastRequest;
}

void AbstractVisualListModel::refresh()
{
    tryQuery();
}
