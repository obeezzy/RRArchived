#include "qmlstockproductdetailrecord.h"
#include "database/databasethread.h"
#include "queryexecutors/stock.h"
#include "utility/stockutils.h"
#include <QDateTime>

QMLStockProductDetailRecord::QMLStockProductDetailRecord(QObject *parent) :
    QMLStockProductDetailRecord(DatabaseThread::instance(), parent)
{}

QMLStockProductDetailRecord::QMLStockProductDetailRecord(DatabaseThread &thread, QObject *parent) :
    AbstractDetailRecord(thread, parent)
{
    connect(this, &QMLStockProductDetailRecord::productIdChanged,
            this, &QMLStockProductDetailRecord::tryQuery);
}

int QMLStockProductDetailRecord::productId() const
{
    return m_product.id;
}

void QMLStockProductDetailRecord::setProductId(int productId)
{
    if (m_product.id == productId)
        return;

    m_product.id = productId;
    emit productIdChanged();
}

int QMLStockProductDetailRecord::categoryId() const
{
    return m_product.category.id;
}

QString QMLStockProductDetailRecord::category() const
{
    return m_product.category.category;
}

QString QMLStockProductDetailRecord::product() const
{
    return m_product.product;
}

QString QMLStockProductDetailRecord::description() const
{
    return m_product.description;
}

bool QMLStockProductDetailRecord::isDivisible() const
{
    return m_product.flags.testFlag(Utility::RecordGroup::Divisible);
}

QUrl QMLStockProductDetailRecord::imageUrl() const
{
    return m_product.imageUrl;
}

double QMLStockProductDetailRecord::quantity() const
{
    return m_product.quantity.toDouble();
}

int QMLStockProductDetailRecord::unitId() const
{
    return m_product.unit.id;
}

QString QMLStockProductDetailRecord::unit() const
{
    return m_product.unit.unit;
}

double QMLStockProductDetailRecord::costPrice() const
{
    return m_product.monies.costPrice.toDouble();
}

double QMLStockProductDetailRecord::retailPrice() const
{
    return m_product.monies.retailPrice.toDouble();
}

QString QMLStockProductDetailRecord::currency() const
{
    return m_product.monies.retailPrice.currency().isoCode();
}

QDateTime QMLStockProductDetailRecord::created() const
{
    return m_product.timestamp.created;
}

QDateTime QMLStockProductDetailRecord::lastEdited() const
{
    return m_product.timestamp.lastEdited;
}

int QMLStockProductDetailRecord::userId() const
{
    return m_product.user.id;
}

QString QMLStockProductDetailRecord::user() const
{
    return m_product.user.user;
}

void QMLStockProductDetailRecord::tryQuery()
{
    if (m_product.id <= 0)
        return;

    setBusy(true);
    emit execute(new StockQuery::FetchStockProduct(m_product.id,
                                                   this));
}

void QMLStockProductDetailRecord::processResult(const QueryResult result)
{
    setBusy(false);

    if (result.isSuccessful()) {
        const auto product = Utility::StockProduct{ result.outcome().toMap().value("product").toMap() };
        setCategoryId(product.category.id);
        setCategory(product.category.category);
        setProduct(product.product);
        setDescription(product.description);
        setDivisible(product.flags.testFlag(Utility::RecordGroup::Divisible));
        setImageUrl(product.imageUrl);
        setQuantity(product.quantity.toDouble());
        setUnitId(product.id);
        setUnit(product.unit.unit);
        setCostPrice(product.monies.costPrice.toDouble());
        setRetailPrice(product.monies.retailPrice.toDouble());
        setCreated(product.timestamp.created);
        setLastEdited(product.timestamp.lastEdited);
        setUserId(product.user.id);
        setUser(product.user.user);
        emit success();
    } else {
        emit error();
    }
}

void QMLStockProductDetailRecord::setCategoryId(int categoryId)
{
    if (m_product.category.id == categoryId)
        return;

    m_product.category.id = categoryId;
    emit categoryIdChanged();
}

void QMLStockProductDetailRecord::setCategory(const QString &category)
{
    if (m_product.category.category == category)
        return;

    m_product.category.category = category;
    emit categoryChanged();
}

void QMLStockProductDetailRecord::setProduct(const QString &product)
{
    if (m_product.product == product)
        return;

    m_product.product = product;
    emit productChanged();
}

void QMLStockProductDetailRecord::setDescription(const QString &description)
{
    if (m_product.description == description)
        return;

    m_product.description = description;
    emit descriptionChanged();
}

void QMLStockProductDetailRecord::setDivisible(bool divisible)
{
    if (m_product.flags.testFlag(Utility::RecordGroup::Divisible) == divisible)
        return;

    m_product.flags.setFlag(Utility::RecordGroup::Divisible, divisible);
    emit divisibleChanged();
}

void QMLStockProductDetailRecord::setImageUrl(const QUrl &imageUrl)
{
    if (m_product.imageUrl == imageUrl)
        return;

    m_product.imageUrl = imageUrl;
    emit imageUrlChanged();
}

void QMLStockProductDetailRecord::setQuantity(double quantity)
{
    if (m_product.quantity == Utility::StockProductQuantity(quantity))
        return;

    m_product.quantity = Utility::StockProductQuantity(quantity);
    emit quantityChanged();
}

void QMLStockProductDetailRecord::setUnitId(int unitId)
{
    if (m_product.unit.id == unitId)
        return;

    m_product.unit.id = unitId;
    emit unitIdChanged();
}

void QMLStockProductDetailRecord::setUnit(const QString &unit)
{
    if (m_product.unit.unit == unit)
        return;

    m_product.unit.unit = unit;
    emit unitChanged();
}

void QMLStockProductDetailRecord::setCostPrice(double costPrice)
{
    if (m_product.monies.costPrice == Utility::Money(costPrice))
        return;

    m_product.monies.costPrice = Utility::Money(costPrice);
    emit costPriceChanged();
}

void QMLStockProductDetailRecord::setRetailPrice(double retailPrice)
{
    if (m_product.monies.retailPrice == Utility::Money(retailPrice))
        return;

    m_product.monies.retailPrice = Utility::Money(retailPrice);
    emit retailPriceChanged();
}

void QMLStockProductDetailRecord::setCreated(const QDateTime &created)
{
    if (m_product.timestamp.created == created)
        return;

    m_product.timestamp.created = created;
    emit createdChanged();
}

void QMLStockProductDetailRecord::setLastEdited(const QDateTime &lastEdited)
{
    if (m_product.timestamp.lastEdited == lastEdited)
        return;

    m_product.timestamp.lastEdited = lastEdited;
    emit lastEditedChanged();
}

void QMLStockProductDetailRecord::setUserId(int userId)
{
    if (m_product.user.id == userId)
        return;

    m_product.user.id = userId;
    emit userIdChanged();
}

void QMLStockProductDetailRecord::setUser(const QString &user)
{
    if (m_product.user.user == user)
        return;

    m_product.user.user = user;
    emit userChanged();
}
