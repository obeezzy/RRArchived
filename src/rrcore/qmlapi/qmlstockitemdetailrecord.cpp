#include "qmlstockitemdetailrecord.h"
#include "database/databasethread.h"
#include "queryexecutors/stock.h"

#include <QDateTime>

QMLStockItemDetailRecord::QMLStockItemDetailRecord(QObject *parent) :
    QMLStockItemDetailRecord(DatabaseThread::instance(), parent)
{}

QMLStockItemDetailRecord::QMLStockItemDetailRecord(DatabaseThread &thread, QObject *parent) :
    AbstractDetailRecord(thread, parent),
    m_itemId(-1),
    m_categoryId(-1),
    m_divisible(false),
    m_quantity(0.0),
    m_unitId(-1),
    m_costPrice(0.0),
    m_retailPrice(0.0),
    m_userId(-1)
{
    connect(this, &QMLStockItemDetailRecord::itemIdChanged, this, &QMLStockItemDetailRecord::tryQuery);
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
    return m_categoryId;
}

QString QMLStockItemDetailRecord::category() const
{
    return m_category;
}

QString QMLStockItemDetailRecord::item() const
{
    return m_item;
}

QString QMLStockItemDetailRecord::description() const
{
    return m_description;
}

bool QMLStockItemDetailRecord::isDivisible() const
{
    return m_divisible;
}

QUrl QMLStockItemDetailRecord::imageUrl() const
{
    return m_imageUrl;
}

double QMLStockItemDetailRecord::quantity() const
{
    return m_quantity;
}

int QMLStockItemDetailRecord::unitId() const
{
    return m_unitId;
}

QString QMLStockItemDetailRecord::unit() const
{
    return m_unit;
}

qreal QMLStockItemDetailRecord::costPrice() const
{
    return m_costPrice;
}

qreal QMLStockItemDetailRecord::retailPrice() const
{
    return m_retailPrice;
}

QString QMLStockItemDetailRecord::currency() const
{
    return m_currency;
}

QDateTime QMLStockItemDetailRecord::created() const
{
    return m_created;
}

QDateTime QMLStockItemDetailRecord::lastEdited() const
{
    return m_lastEdited;
}

int QMLStockItemDetailRecord::userId() const
{
    return m_userId;
}

QString QMLStockItemDetailRecord::user() const
{
    return m_user;
}

void QMLStockItemDetailRecord::tryQuery()
{
    if (m_itemId <= 0)
        return;

    setBusy(true);
    emit execute(new StockQuery::ViewStockItemDetails(m_itemId, this));
}

void QMLStockItemDetailRecord::processResult(const QueryResult result)
{
    if (result.request().receiver() != this)
        return;

    setBusy(false);

    if (result.isSuccessful()) {
        const QVariantMap &record = result.outcome().toMap().value("item").toMap();
        setCategoryId(record.value("category_id").toInt());
        setCategory(record.value("category").toString());
        setItem(record.value("item").toString());
        setDescription(record.value("description").toString());
        setDivisible(record.value("divisible").toBool());
        setImageUrl(record.value("image_url").toString());
        setQuantity(record.value("quantity").toDouble());
        setUnitId(record.value("unit_id").toInt());
        setUnit(record.value("unit").toString());
        setCostPrice(record.value("cost_price").toDouble());
        setRetailPrice(record.value("retail_price").toDouble());
        setCurrency(record.value("currency").toString());
        setCreated(record.value("created").toDateTime());
        setLastEdited(record.value("last_edited").toDateTime());
        setUserId(record.value("user_id").toInt());
        setUser(record.value("user").toString());
        emit success();
    } else {
        emit error();
    }
}

void QMLStockItemDetailRecord::setCategoryId(int categoryId)
{
    if (m_categoryId == categoryId)
        return;

    m_categoryId = categoryId;
    emit categoryIdChanged();
}

void QMLStockItemDetailRecord::setCategory(const QString &category)
{
    if (m_category == category)
        return;

    m_category = category;
    emit categoryChanged();
}

void QMLStockItemDetailRecord::setItem(const QString &item)
{
    if (m_item == item)
        return;

    m_item = item;
    emit itemChanged();
}

void QMLStockItemDetailRecord::setDescription(const QString &description)
{
    if (m_description == description)
        return;

    m_description = description;
    emit descriptionChanged();
}

void QMLStockItemDetailRecord::setDivisible(bool divisible)
{
    if (m_divisible == divisible)
        return;

    m_divisible = divisible;
    emit divisibleChanged();
}

void QMLStockItemDetailRecord::setImageUrl(const QUrl &imageUrl)
{
    if (m_imageUrl == imageUrl)
        return;

    m_imageUrl = imageUrl;
    emit imageUrlChanged();
}

void QMLStockItemDetailRecord::setQuantity(double quantity)
{
    if (m_quantity == quantity)
        return;

    m_quantity = quantity;
    emit quantityChanged();
}

void QMLStockItemDetailRecord::setUnitId(int unitId)
{
    if (m_unitId == unitId)
        return;

    m_unitId = unitId;
    emit unitIdChanged();
}

void QMLStockItemDetailRecord::setUnit(const QString &unit)
{
    if (m_unit == unit)
        return;

    m_unit = unit;
    emit unitChanged();
}

void QMLStockItemDetailRecord::setCostPrice(qreal costPrice)
{
    if (m_costPrice == costPrice)
        return;

    m_costPrice = costPrice;
    emit costPriceChanged();
}

void QMLStockItemDetailRecord::setRetailPrice(qreal retailPrice)
{
    if (m_retailPrice == retailPrice)
        return;

    m_retailPrice = retailPrice;
    emit retailPriceChanged();
}

void QMLStockItemDetailRecord::setCurrency(const QString &currency)
{
    if (m_currency == currency)
        return;

    m_currency = currency;
    emit currencyChanged();
}

void QMLStockItemDetailRecord::setCreated(const QDateTime &created)
{
    if (m_created == created)
        return;

    m_created = created;
    emit createdChanged();
}

void QMLStockItemDetailRecord::setLastEdited(const QDateTime &lastEdited)
{
    if (m_lastEdited == lastEdited)
        return;

    m_lastEdited = lastEdited;
    emit lastEditedChanged();
}

void QMLStockItemDetailRecord::setUserId(int userId)
{
    if (m_userId == userId)
        return;

    m_userId = userId;
    emit userIdChanged();
}

void QMLStockItemDetailRecord::setUser(const QString &user)
{
    if (m_user == user)
        return;

    m_user = user;
    emit userChanged();
}
