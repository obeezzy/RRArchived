#include "qmlstockproductdetailrecord.h"
#include "database/databasethread.h"
#include "queryexecutors/stock.h"
#include "utility/stockutils.h"

#include <QDateTime>

QMLStockProductDetailRecord::QMLStockProductDetailRecord(QObject *parent) :
    QMLStockProductDetailRecord(DatabaseThread::instance(), parent)
{}

QMLStockProductDetailRecord::QMLStockProductDetailRecord(DatabaseThread &thread, QObject *parent) :
    AbstractDetailRecord(thread, parent),
    m_productId(-1),
    m_categoryId(-1),
    m_divisible(false),
    m_quantity(0.0),
    m_unitId(-1),
    m_costPrice(0.0),
    m_retailPrice(0.0),
    m_userId(-1)
{
    connect(this, &QMLStockProductDetailRecord::productIdChanged, this, &QMLStockProductDetailRecord::tryQuery);
}

int QMLStockProductDetailRecord::productId() const
{
    return m_productId;
}

void QMLStockProductDetailRecord::setProductId(int productId)
{
    if (m_productId == productId)
        return;

    m_productId = productId;
    emit productIdChanged();
}

int QMLStockProductDetailRecord::categoryId() const
{
    return m_categoryId;
}

QString QMLStockProductDetailRecord::category() const
{
    return m_category;
}

QString QMLStockProductDetailRecord::product() const
{
    return m_product;
}

QString QMLStockProductDetailRecord::description() const
{
    return m_description;
}

bool QMLStockProductDetailRecord::isDivisible() const
{
    return m_divisible;
}

QUrl QMLStockProductDetailRecord::imageUrl() const
{
    return m_imageUrl;
}

double QMLStockProductDetailRecord::quantity() const
{
    return m_quantity;
}

int QMLStockProductDetailRecord::unitId() const
{
    return m_unitId;
}

QString QMLStockProductDetailRecord::unit() const
{
    return m_unit;
}

qreal QMLStockProductDetailRecord::costPrice() const
{
    return m_costPrice;
}

qreal QMLStockProductDetailRecord::retailPrice() const
{
    return m_retailPrice;
}

QString QMLStockProductDetailRecord::currency() const
{
    return m_currency;
}

QDateTime QMLStockProductDetailRecord::created() const
{
    return m_created;
}

QDateTime QMLStockProductDetailRecord::lastEdited() const
{
    return m_lastEdited;
}

int QMLStockProductDetailRecord::userId() const
{
    return m_userId;
}

QString QMLStockProductDetailRecord::user() const
{
    return m_user;
}

void QMLStockProductDetailRecord::tryQuery()
{
    if (m_productId <= 0)
        return;

    setBusy(true);
    emit execute(new StockQuery::FetchStockProduct(m_productId,
                                                   this));
}

void QMLStockProductDetailRecord::processResult(const QueryResult result)
{
    if (result.request().receiver() != this)
        return;

    setBusy(false);

    if (result.isSuccessful()) {
        const StockProduct &product{result.outcome().toMap().value("product").toMap()};
        setCategoryId(product.category.id);
        setCategory(product.category.category);
        setProduct(product.product);
        setDescription(product.description);
        setDivisible(product.divisible);
        setImageUrl(product.imageUrl);
        setQuantity(product.quantity);
        setUnitId(product.id);
        setUnit(product.unit.unit);
        setCostPrice(product.costPrice);
        setRetailPrice(product.retailPrice);
        setCurrency(product.currency);
        setCreated(product.created);
        setLastEdited(product.lastEdited);
        setUserId(product.user.id);
        setUser(product.user.user);
        emit success();
    } else {
        emit error();
    }
}

void QMLStockProductDetailRecord::setCategoryId(int categoryId)
{
    if (m_categoryId == categoryId)
        return;

    m_categoryId = categoryId;
    emit categoryIdChanged();
}

void QMLStockProductDetailRecord::setCategory(const QString &category)
{
    if (m_category == category)
        return;

    m_category = category;
    emit categoryChanged();
}

void QMLStockProductDetailRecord::setProduct(const QString &product)
{
    if (m_product == product)
        return;

    m_product = product;
    emit productChanged();
}

void QMLStockProductDetailRecord::setDescription(const QString &description)
{
    if (m_description == description)
        return;

    m_description = description;
    emit descriptionChanged();
}

void QMLStockProductDetailRecord::setDivisible(bool divisible)
{
    if (m_divisible == divisible)
        return;

    m_divisible = divisible;
    emit divisibleChanged();
}

void QMLStockProductDetailRecord::setImageUrl(const QUrl &imageUrl)
{
    if (m_imageUrl == imageUrl)
        return;

    m_imageUrl = imageUrl;
    emit imageUrlChanged();
}

void QMLStockProductDetailRecord::setQuantity(double quantity)
{
    if (m_quantity == quantity)
        return;

    m_quantity = quantity;
    emit quantityChanged();
}

void QMLStockProductDetailRecord::setUnitId(int unitId)
{
    if (m_unitId == unitId)
        return;

    m_unitId = unitId;
    emit unitIdChanged();
}

void QMLStockProductDetailRecord::setUnit(const QString &unit)
{
    if (m_unit == unit)
        return;

    m_unit = unit;
    emit unitChanged();
}

void QMLStockProductDetailRecord::setCostPrice(qreal costPrice)
{
    if (m_costPrice == costPrice)
        return;

    m_costPrice = costPrice;
    emit costPriceChanged();
}

void QMLStockProductDetailRecord::setRetailPrice(qreal retailPrice)
{
    if (m_retailPrice == retailPrice)
        return;

    m_retailPrice = retailPrice;
    emit retailPriceChanged();
}

void QMLStockProductDetailRecord::setCurrency(const QString &currency)
{
    if (m_currency == currency)
        return;

    m_currency = currency;
    emit currencyChanged();
}

void QMLStockProductDetailRecord::setCreated(const QDateTime &created)
{
    if (m_created == created)
        return;

    m_created = created;
    emit createdChanged();
}

void QMLStockProductDetailRecord::setLastEdited(const QDateTime &lastEdited)
{
    if (m_lastEdited == lastEdited)
        return;

    m_lastEdited = lastEdited;
    emit lastEditedChanged();
}

void QMLStockProductDetailRecord::setUserId(int userId)
{
    if (m_userId == userId)
        return;

    m_userId = userId;
    emit userIdChanged();
}

void QMLStockProductDetailRecord::setUser(const QString &user)
{
    if (m_user == user)
        return;

    m_user = user;
    emit userChanged();
}
