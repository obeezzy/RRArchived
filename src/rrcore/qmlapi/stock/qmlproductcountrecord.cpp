#include "qmlproductcountrecord.h"
#include "database/databasethread.h"
#include "database/queryrequest.h"
#include "database/queryresult.h"
#include "query/stock/fetchproductcount.h"
#include "query/stock/filterproductcount.h"

QMLProductCountRecord::QMLProductCountRecord(QObject* parent)
    : QMLProductCountRecord(DatabaseThread::instance(), parent)
{}

QMLProductCountRecord::QMLProductCountRecord(DatabaseThread& thread,
                                             QObject* parent)
    : AbstractDetailRecord(thread, parent)
{
    connect(this, &QMLProductCountRecord::filterTextChanged, this,
            &QMLProductCountRecord::filter);
    connect(this, &QMLProductCountRecord::filterColumnChanged, this,
            &QMLProductCountRecord::filter);
}

int QMLProductCountRecord::categoryId() const
{
    return m_categoryId;
}

void QMLProductCountRecord::setCategoryId(int categoryId)
{
    if (m_categoryId == categoryId)
        return;

    m_categoryId = categoryId;
    emit categoryIdChanged();
}

QString QMLProductCountRecord::filterText() const
{
    return m_filterCriteria.text;
}

void QMLProductCountRecord::setFilterText(const QString& filterText)
{
    if (m_filterCriteria.text == filterText)
        return;

    m_filterCriteria.text = filterText;
    emit filterTextChanged();
}

int QMLProductCountRecord::filterColumn() const
{
    return m_filterCriteria.columnIndex;
}

void QMLProductCountRecord::setFilterColumn(int filterColumn)
{
    if (m_filterCriteria.columnIndex == filterColumn)
        return;

    m_filterCriteria.columnIndex = filterColumn;
    m_filterCriteria.column = columnName(filterColumn);
    emit filterColumnChanged();
}

int QMLProductCountRecord::productCount() const
{
    return m_productCount;
}

void QMLProductCountRecord::refresh()
{
    tryQuery();
}

void QMLProductCountRecord::tryQuery()
{
    setBusy(true);

    if (m_filterCriteria.isValid())
        emit execute(new Query::Stock::FilterProductCount(m_filterCriteria,
                                                          m_categoryId, this));
    else
        emit execute(new Query::Stock::FetchProductCount(m_categoryId, this));
}

void QMLProductCountRecord::processResult(const QueryResult result)
{
    setBusy(false);

    if (result.isSuccessful()) {
        setProductCount(
            result.outcome().toMap().value("product_count").toInt());
        emit success();
    } else {
        emit error();
    }
}

void QMLProductCountRecord::setProductCount(int productCount)
{
    if (m_productCount == productCount)
        return;

    m_productCount = productCount;
    emit productCountChanged();
}

void QMLProductCountRecord::filter()
{
    tryQuery();
}

QString QMLProductCountRecord::columnName(int column) const
{
    switch (column) {
        case CategoryColumn:
            return QStringLiteral("product_category");
        case ProductColumn:
            return QStringLiteral("product");
    }

    return QString();
}
