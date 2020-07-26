#include "qmlproductpusher.h"
#include <QUrl>
#include "database/databaseerror.h"
#include "database/databasethread.h"
#include "query/stock/addproduct.h"
#include "query/stock/updateproduct.h"

QMLProductPusher::QMLProductPusher(QObject* parent)
    : QMLProductPusher(DatabaseThread::instance(), parent)
{}

QMLProductPusher::QMLProductPusher(DatabaseThread& thread, QObject* parent)
    : AbstractPusher(thread, parent)
{}

bool QMLProductPusher::isExistingProduct() const
{
    return m_product.id > 0;
}

int QMLProductPusher::productId() const
{
    return m_product.id;
}

void QMLProductPusher::setProductId(int productId)
{
    if (m_product.id == productId)
        return;

    m_product.id = productId;
    emit productIdChanged();
}

QUrl QMLProductPusher::imageUrl() const
{
    return m_product.imageUrl;
}

void QMLProductPusher::setImageUrl(const QUrl& imageUrl)
{
    if (m_product.imageUrl == imageUrl)
        return;

    m_product.imageUrl = imageUrl;
    emit imageUrlChanged();
}

QString QMLProductPusher::category() const
{
    return m_product.category.category;
}

void QMLProductPusher::setCategory(const QString& category)
{
    if (m_product.category.category == category)
        return;

    m_product.category.category = category;
    emit categoryChanged();
}

QString QMLProductPusher::product() const
{
    return m_product.product;
}

void QMLProductPusher::setProduct(const QString& product)
{
    if (m_product.product == product)
        return;

    m_product.product = product;
    emit productChanged();
}

QString QMLProductPusher::description() const
{
    return m_product.description;
}

void QMLProductPusher::setDescription(const QString& description)
{
    if (m_product.description == description)
        return;

    m_product.description = description;
    emit descriptionChanged();
}

double QMLProductPusher::quantity() const
{
    return m_product.quantity.toDouble();
}

void QMLProductPusher::setQuantity(double quantity)
{
    if (m_product.quantity == Utility::Stock::ProductQuantity(quantity))
        return;

    m_product.quantity = Utility::Stock::ProductQuantity(quantity);
    emit quantityChanged();
}

QString QMLProductPusher::unit() const
{
    return m_product.unit.unit;
}

void QMLProductPusher::setUnit(const QString& unit)
{
    if (m_product.unit.unit == unit)
        return;

    m_product.unit.unit = unit;
    emit unitChanged();
}

QString QMLProductPusher::categoryNote() const
{
    return m_product.category.note.note;
}

void QMLProductPusher::setCategoryNote(const QString& note)
{
    if (m_product.category.note.note == note)
        return;

    m_product.category.note.note = note;
    emit categoryNoteChanged();
}

QString QMLProductPusher::productNote() const
{
    return m_product.note.note;
}

void QMLProductPusher::setProductNote(const QString& note)
{
    if (m_product.note.note == note)
        return;

    m_product.note.note = note;
    emit productNoteChanged();
}

bool QMLProductPusher::isTracked() const
{
    return m_product.flags.testFlag(Utility::RecordGroup::Tracked);
}

void QMLProductPusher::setTracked(bool tracked)
{
    if (m_product.flags.testFlag(Utility::RecordGroup::Tracked) == tracked)
        return;

    m_product.flags.setFlag(Utility::RecordGroup::Tracked, tracked);
    emit trackedChanged();
}

bool QMLProductPusher::isDivisible() const
{
    return m_product.flags.testFlag(Utility::RecordGroup::Divisible);
}

void QMLProductPusher::setDivisible(bool divisible)
{
    if (m_product.flags.testFlag(Utility::RecordGroup::Divisible) == divisible)
        return;

    m_product.flags.setFlag(Utility::RecordGroup::Divisible, divisible);
    emit divisibleChanged();
}

double QMLProductPusher::costPrice() const
{
    return m_product.monies.costPrice.toDouble();
}

void QMLProductPusher::setCostPrice(double costPrice)
{
    if (m_product.monies.costPrice == Utility::Money(costPrice))
        return;

    m_product.monies.costPrice = Utility::Money(costPrice);
    emit costPriceChanged();
}

double QMLProductPusher::retailPrice() const
{
    return m_product.monies.retailPrice.toDouble();
}

void QMLProductPusher::setRetailPrice(double retailPrice)
{
    if (m_product.monies.retailPrice == Utility::Money(retailPrice))
        return;

    m_product.monies.retailPrice = Utility::Money(retailPrice);
    emit retailPriceChanged();
}

bool QMLProductPusher::canProcessResult(const QueryResult& result)
{
    Q_UNUSED(result)
    return true;
}

void QMLProductPusher::push()
{
    setBusy(true);

    if (isExistingProduct())
        emit execute(new Query::Stock::UpdateProduct(m_product, this));
    else
        emit execute(new Query::Stock::AddProduct(m_product, this));
}

void QMLProductPusher::processResult(const QueryResult result)
{
    setBusy(false);

    if (result.isSuccessful()) {
        if (result.request().command() == Query::Stock::AddProduct::COMMAND)
            emit success(ModelResult{AddProductSuccess});
        else if (result.request().command() ==
                 Query::Stock::UpdateProduct::COMMAND)
            emit success(ModelResult{UpdateProductSuccess});
    } else {
        switch (result.errorCode()) {
            case static_cast<int>(
                DatabaseError::MySqlErrorCode::UserDefinedException):
                emit error(ModelResult{DuplicateEntryError});
                break;
            case static_cast<int>(DatabaseError::QueryErrorCode::ImageTooLarge):
                emit error(ModelResult{ImageTooLargeError});
                break;
            default:
                emit error();
                break;
        }
    }
}
