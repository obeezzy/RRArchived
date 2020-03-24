#include "qmlstockproductcountrecord.h"
#include "database/databasethread.h"
#include "database/queryrequest.h"
#include "database/queryresult.h"
#include "queryexecutors/stock.h"

QMLStockProductCountRecord::QMLStockProductCountRecord(QObject *parent) :
    QMLStockProductCountRecord(DatabaseThread::instance(), parent)
{}

QMLStockProductCountRecord::QMLStockProductCountRecord(DatabaseThread &thread,
                                                       QObject *parent) :
    AbstractDetailRecord(thread, parent)
{
    connect(this, &QMLStockProductCountRecord::filterTextChanged,
            this, &QMLStockProductCountRecord::filter);
    connect(this, &QMLStockProductCountRecord::filterColumnChanged,
            this, &QMLStockProductCountRecord::filter);
}

int QMLStockProductCountRecord::categoryId() const
{
    return m_categoryId;
}

void QMLStockProductCountRecord::setCategoryId(int categoryId)
{
    if (m_categoryId == categoryId)
        return;

    m_categoryId = categoryId;
    emit categoryIdChanged();
}

QString QMLStockProductCountRecord::filterText() const
{
    return m_filterCriteria.text;
}

void QMLStockProductCountRecord::setFilterText(const QString &filterText)
{
    if (m_filterCriteria.text == filterText)
        return;

    m_filterCriteria.text = filterText;
    emit filterTextChanged();
}

int QMLStockProductCountRecord::filterColumn() const
{
    return m_filterCriteria.columnIndex;
}

void QMLStockProductCountRecord::setFilterColumn(int filterColumn)
{
    if (m_filterCriteria.columnIndex == filterColumn)
        return;

    m_filterCriteria.columnIndex = filterColumn;
    m_filterCriteria.column = columnName(filterColumn);
    emit filterColumnChanged();
}

int QMLStockProductCountRecord::productCount() const
{
    return m_productCount;
}

void QMLStockProductCountRecord::refresh()
{
    tryQuery();
}

void QMLStockProductCountRecord::tryQuery()
{
    setBusy(true);

    if (m_filterCriteria.isValid())
        emit execute(new StockQuery::FilterStockProductCount(
                         m_filterCriteria,
                         m_categoryId,
                         this));
    else
        emit execute(new StockQuery::FetchStockProductCount(m_categoryId,
                                                            this));
}

void QMLStockProductCountRecord::processResult(const QueryResult result)
{
    if (result.request().receiver() != this)
        return;

    setBusy(false);

    if (result.isSuccessful()) {
        setProductCount(result.outcome().toMap().value("product_count").toInt());
        emit success();
    } else {
        emit error();
    }
}

void QMLStockProductCountRecord::setProductCount(int productCount)
{
    if (m_productCount == productCount)
        return;

    m_productCount = productCount;
    emit productCountChanged();
}

void QMLStockProductCountRecord::filter()
{
    tryQuery();
}

QString QMLStockProductCountRecord::columnName(int column) const
{
    switch (column) {
    case CategoryColumn:
        return QStringLiteral("product_category");
    case ProductColumn:
        return QStringLiteral("product");
    }

    return QString();
}
