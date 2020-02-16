#include "qmlstockproductpusher.h"
#include "database/databasethread.h"
#include "database/databaseerror.h"
#include "queryexecutors/stock.h"
#include "singletons/settings.h"
#include "utility/stockutils.h"
#include <QUrl>

QMLStockProductPusher::QMLStockProductPusher(QObject *parent) :
    QMLStockProductPusher(DatabaseThread::instance(), parent)
{}

QMLStockProductPusher::QMLStockProductPusher(DatabaseThread &thread, QObject *parent) :
    AbstractPusher(thread, parent)
{

}

bool QMLStockProductPusher::isExistingProduct() const
{
    return m_product.id > 0;
}

int QMLStockProductPusher::productId() const
{
    return m_product.id;
}

void QMLStockProductPusher::setProductId(int productId)
{
    if (m_product.id == productId)
        return;

    m_product.id = productId;
    emit productIdChanged();
}

QUrl QMLStockProductPusher::imageUrl() const
{
    return m_product.imageUrl;
}

void QMLStockProductPusher::setImageUrl(const QUrl &imageUrl)
{
    if (m_product.imageUrl == imageUrl)
        return;

    m_product.imageUrl = imageUrl;
    emit imageUrlChanged();
}

QString QMLStockProductPusher::category() const
{
    return m_product.category.category;
}

void QMLStockProductPusher::setCategory(const QString &category)
{
    if (m_product.category.category == category)
        return;

    m_product.category.category = category;
    emit categoryChanged();
}

QString QMLStockProductPusher::product() const
{
    return m_product.product;
}

void QMLStockProductPusher::setProduct(const QString &product)
{
    if (m_product.product == product)
        return;

    m_product.product = product;
    emit productChanged();
}

QString QMLStockProductPusher::description() const
{
    return m_product.description;
}

void QMLStockProductPusher::setDescription(const QString &description)
{
    if (m_product.description == description)
        return;

    m_product.description = description;
    emit descriptionChanged();
}

double QMLStockProductPusher::quantity() const
{
    return m_product.quantity;
}

void QMLStockProductPusher::setQuantity(double quantity)
{
    if (m_product.quantity == quantity)
        return;

    m_product.quantity = quantity;
    emit quantityChanged();
}

QString QMLStockProductPusher::unit() const
{
    return m_product.unit.unit;
}

void QMLStockProductPusher::setUnit(const QString &unit)
{
    if (m_product.unit.unit == unit)
        return;

    m_product.unit.unit = unit;
    emit unitChanged();
}

QString QMLStockProductPusher::categoryNote() const
{
    return m_product.category.note.note;
}

void QMLStockProductPusher::setCategoryNote(const QString &note)
{
    if (m_product.category.note.note == note)
        return;

    m_product.category.note.note = note;
    emit categoryNoteChanged();
}

QString QMLStockProductPusher::productNote() const
{
    return m_product.note.note;
}

void QMLStockProductPusher::setProductNote(const QString &note)
{
    if (m_product.note.note == note)
        return;

    m_product.note.note = note;
    emit productNoteChanged();
}

bool QMLStockProductPusher::isTracked() const
{
    return m_product.flags.testFlag(Utility::RecordGroup::Tracked);
}

void QMLStockProductPusher::setTracked(bool tracked)
{
    if (m_product.flags.testFlag(Utility::RecordGroup::Tracked) == tracked)
        return;

    m_product.flags.setFlag(Utility::RecordGroup::Tracked, tracked);
    emit trackedChanged();
}

bool QMLStockProductPusher::isDivisible() const
{
    return m_product.flags.testFlag(Utility::RecordGroup::Divisible);
}

void QMLStockProductPusher::setDivisible(bool divisible)
{
    if (m_product.flags.testFlag(Utility::RecordGroup::Divisible))
        return;

    m_product.flags.setFlag(Utility::RecordGroup::Divisible, divisible);
    emit divisibleChanged();
}

double QMLStockProductPusher::costPrice() const
{
    return m_product.costPrice;
}

void QMLStockProductPusher::setCostPrice(double costPrice)
{
    if (m_product.costPrice == costPrice)
        return;

    m_product.costPrice = costPrice;
    emit costPriceChanged();
}

double QMLStockProductPusher::retailPrice() const
{
    return m_product.retailPrice;
}

void QMLStockProductPusher::setRetailPrice(double retailPrice)
{
    if (m_product.retailPrice == retailPrice)
        return;

    m_product.retailPrice = retailPrice;
    emit retailPriceChanged();
}

void QMLStockProductPusher::push()
{
    setBusy(true);

    if (isExistingProduct())
        emit execute(new StockQuery::UpdateStockProduct(m_product,
                                                        this));
    else
        emit execute(new StockQuery::AddStockProduct(m_product,
                                                     this));
}

void QMLStockProductPusher::processResult(const QueryResult result)
{
    if (this != result.request().receiver())
        return;

    setBusy(false);

    if (result.isSuccessful()) {
        if (result.request().command() == StockQuery::AddStockProduct::COMMAND)
            emit success(AddProductSuccess);
        else if (result.request().command() == StockQuery::UpdateStockProduct::COMMAND)
            emit success(UpdateProductSuccess);
    } else {
        switch (result.errorCode()) {
        case static_cast<int>(DatabaseError::MySqlErrorCode::UserDefinedException):
            emit error(DuplicateEntryError);
            break;
        case static_cast<int>(DatabaseError::QueryErrorCode::ImageTooLarge):
            emit error(ImageTooLargeError);
            break;
        default:
            emit error();
            break;
        }
    }
}


