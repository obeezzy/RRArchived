#include "qmlstockitemdetailrecord.h"
#include <QDateTime>

QMLStockItemDetailRecord::QMLStockItemDetailRecord(QObject *parent) :
    AbstractDetailRecord(parent),
    m_itemId(-1)
{

}

int QMLStockItemDetailRecord::itemId() const
{
    return m_itemId;
}

void QMLStockItemDetailRecord::setItemId(int itemId)
{
    if (m_itemId == itemId)
        return;

    m_itemId = itemId;
    emit itemIdChanged();
}

int QMLStockItemDetailRecord::categoryId() const
{
    return m_record.value("category_id", -1).toInt();
}

QString QMLStockItemDetailRecord::category() const
{
    return m_record.value("category").toString();
}

QString QMLStockItemDetailRecord::item() const
{
    return m_record.value("item").toString();
}

QString QMLStockItemDetailRecord::description() const
{
    return m_record.value("description").toString();
}

bool QMLStockItemDetailRecord::isDivisible() const
{
    return m_record.value("divisible").toBool();
}

QString QMLStockItemDetailRecord::imageSource() const
{
    return m_record.value("image_source").toString();
}

double QMLStockItemDetailRecord::quantity() const
{
    return m_record.value("quantity").toDouble();
}

int QMLStockItemDetailRecord::unitId() const
{
    return m_record.value("unit_id").toInt();
}

QString QMLStockItemDetailRecord::unit() const
{
    return m_record.value("unit").toString();
}

qreal QMLStockItemDetailRecord::costPrice() const
{
    return m_record.value("cost_price").toDouble();
}

qreal QMLStockItemDetailRecord::retailPrice() const
{
    return m_record.value("retail_price").toDouble();
}

QString QMLStockItemDetailRecord::currency() const
{
    return m_record.value("currency").toString();
}

QDateTime QMLStockItemDetailRecord::created() const
{
    return m_record.value("created").toDateTime();
}

QDateTime QMLStockItemDetailRecord::lastEdited() const
{
    return m_record.value("last_edited").toDateTime();
}

int QMLStockItemDetailRecord::userId() const
{
    return m_record.value("user_id", -1).toInt();
}

QString QMLStockItemDetailRecord::user() const
{
    return m_record.value("user").toString();
}

void QMLStockItemDetailRecord::tryQuery()
{
    if (m_itemId <= 0)
        return;

    setBusy(true);
    QueryRequest request(this);
    request.setCommand("view_stock_item_details", { { "item_id", m_itemId } }, QueryRequest::Stock);
    emit executeRequest(request);
}

void QMLStockItemDetailRecord::processResult(const QueryResult result)
{
    if (result.request().receiver() != this)
        return;

    setBusy(false);

    if (result.isSuccessful()) {
        m_record = result.outcome().toMap().value("item").toMap();
        emit success();
    } else {
        emit error();
    }

    reset();
}

void QMLStockItemDetailRecord::reset()
{
    emit itemIdChanged();
    emit categoryIdChanged();
    emit categoryChanged();
    emit itemChanged();
    emit descriptionChanged();
    emit divisibleChanged();
    emit imageSourceChanged();
    emit quantityChanged();
    emit unitIdChanged();
    emit unitChanged();
    emit costPriceChanged();
    emit retailPriceChanged();
    emit currencyChanged();
    emit createdChanged();
    emit lastEditedChanged();
    emit userIdChanged();
    emit userChanged();
}
