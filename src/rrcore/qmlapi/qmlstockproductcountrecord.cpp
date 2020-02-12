#include "qmlstockproductcountrecord.h"
#include "database/databasethread.h"
#include "database/queryrequest.h"
#include "database/queryresult.h"
#include "queryexecutors/stock.h"

QMLStockProductCountRecord::QMLStockProductCountRecord(QObject *parent) :
    QMLStockProductCountRecord(DatabaseThread::instance(), parent)
{}

QMLStockProductCountRecord::QMLStockProductCountRecord(DatabaseThread &thread, QObject *parent) :
    AbstractDetailRecord(thread, parent),
    m_categoryId(-1),
    m_filterColumn(-1),
    m_productCount(0)
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
    return m_filterText;
}

void QMLStockProductCountRecord::setFilterText(const QString &filterText)
{
    if (m_filterText == filterText)
        return;

    m_filterText = filterText;
    emit filterTextChanged();
}

int QMLStockProductCountRecord::filterColumn() const
{
    return m_filterColumn;
}

void QMLStockProductCountRecord::setFilterColumn(int filterColumn)
{
    if (m_filterColumn == filterColumn)
        return;

    m_filterColumn = filterColumn;
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

    if (!m_filterText.trimmed().isEmpty() && m_filterColumn > -1)
        emit execute(new StockQuery::FilterStockProductCount(
                         FilterCriteria {
                             columnName(m_filterColumn),
                             m_filterText
                         },
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
