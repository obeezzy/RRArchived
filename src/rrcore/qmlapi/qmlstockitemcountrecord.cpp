#include "qmlstockitemcountrecord.h"
#include "database/databasethread.h"
#include "database/queryrequest.h"
#include "database/queryresult.h"

QMLStockItemCountRecord::QMLStockItemCountRecord(QObject *parent) :
    QMLStockItemCountRecord(DatabaseThread::instance(), parent)
{}

QMLStockItemCountRecord::QMLStockItemCountRecord(DatabaseThread &thread, QObject *parent) :
    AbstractDetailRecord(thread, parent),
    m_categoryId(-1),
    m_filterColumn(-1),
    m_itemCount(0)
{
    connect(this, &QMLStockItemCountRecord::filterTextChanged, this, &QMLStockItemCountRecord::filter);
    connect(this, &QMLStockItemCountRecord::filterColumnChanged, this, &QMLStockItemCountRecord::filter);
}

int QMLStockItemCountRecord::categoryId() const
{
    return m_categoryId;
}

void QMLStockItemCountRecord::setCategoryId(int categoryId)
{
    if (m_categoryId == categoryId)
        return;

    m_categoryId = categoryId;
    emit categoryIdChanged();
}

QString QMLStockItemCountRecord::filterText() const
{
    return m_filterText;
}

void QMLStockItemCountRecord::setFilterText(const QString &filterText)
{
    if (m_filterText == filterText)
        return;

    m_filterText = filterText;
    emit filterTextChanged();
}

int QMLStockItemCountRecord::filterColumn() const
{
    return m_filterColumn;
}

void QMLStockItemCountRecord::setFilterColumn(int filterColumn)
{
    if (m_filterColumn == filterColumn)
        return;

    m_filterColumn = filterColumn;
    emit filterColumnChanged();
}

int QMLStockItemCountRecord::itemCount() const
{
    return m_itemCount;
}

void QMLStockItemCountRecord::tryQuery()
{
    setBusy(true);

    QueryRequest request(this);
    if (!m_filterText.trimmed().isEmpty() && m_filterColumn > -1)
        request.setCommand("filter_stock_item_count", {
                               { "category_id", m_categoryId },
                               { "filter_text", m_filterText },
                               { "filter_column", columnName(m_filterColumn) }
                           }, QueryRequest::Stock);
    else
        request.setCommand("view_stock_item_count", { }, QueryRequest::Stock);

    emit executeRequest(request);
}

void QMLStockItemCountRecord::processResult(const QueryResult result)
{
    if (result.request().receiver() != this)
        return;

    setBusy(false);

    if (result.isSuccessful()) {
        setItemCount(result.outcome().toMap().value("item_count").toInt());
        emit success();
    } else {
        emit error();
    }
}

void QMLStockItemCountRecord::setItemCount(int itemCount)
{
    if (m_itemCount == itemCount)
        return;

    m_itemCount = itemCount;
    emit itemCountChanged();
}

void QMLStockItemCountRecord::filter()
{
    tryQuery();
}

QString QMLStockItemCountRecord::columnName(int column) const
{
    switch (column) {
    case CategoryColumn:
        return "category";
    case ItemColumn:
        return "item";
    }

    return QString();
}
