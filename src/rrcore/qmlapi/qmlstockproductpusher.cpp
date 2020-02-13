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
    AbstractPusher(thread, parent),
    m_productId(-1),
    m_quantity(0.0),
    m_tracked(false),
    m_divisible(false),
    m_costPrice(0.0),
    m_retailPrice(0.0),
    m_baseUnitEquivalent(1.0)
{

}

bool QMLStockProductPusher::isExistingProduct() const
{
    return m_productId > 0;
}

int QMLStockProductPusher::productId() const
{
    return m_productId;
}

void QMLStockProductPusher::setProductId(int productId)
{
    if (m_productId == productId)
        return;

    m_productId = productId;
    emit productIdChanged();
}

QUrl QMLStockProductPusher::imageUrl() const
{
    return m_imageUrl;
}

void QMLStockProductPusher::setImageUrl(const QUrl &imageUrl)
{
    if (m_imageUrl == imageUrl)
        return;

    m_imageUrl = imageUrl;
    emit imageUrlChanged();
}

QString QMLStockProductPusher::category() const
{
    return m_category;
}

void QMLStockProductPusher::setCategory(const QString &category)
{
    if (m_category == category)
        return;

    m_category = category;
    emit categoryChanged();
}

QString QMLStockProductPusher::product() const
{
    return m_product;
}

void QMLStockProductPusher::setProduct(const QString &product)
{
    if (m_product == product)
        return;

    m_product = product;
    emit productChanged();
}

QString QMLStockProductPusher::description() const
{
    return m_description;
}

void QMLStockProductPusher::setDescription(const QString &description)
{
    if (m_description == description)
        return;

    m_description = description;
    emit descriptionChanged();
}

double QMLStockProductPusher::quantity() const
{
    return m_quantity;
}

void QMLStockProductPusher::setQuantity(double quantity)
{
    if (m_quantity == quantity)
        return;

    m_quantity = quantity;
    emit quantityChanged();
}

QString QMLStockProductPusher::unit() const
{
    return m_unit;
}

void QMLStockProductPusher::setUnit(const QString &unit)
{
    if (m_unit == unit)
        return;

    m_unit = unit;
    emit unitChanged();
}

QString QMLStockProductPusher::categoryNote() const
{
    return m_categoryNote;
}

void QMLStockProductPusher::setCategoryNote(const QString &note)
{
    if (m_categoryNote == note)
        return;

    m_categoryNote = note;
    emit categoryNoteChanged();
}

QString QMLStockProductPusher::productNote() const
{
    return m_productNote;
}

void QMLStockProductPusher::setProductNote(const QString &note)
{
    if (m_productNote == note)
        return;

    m_productNote = note;
    emit productNoteChanged();
}

bool QMLStockProductPusher::isTracked() const
{
    return m_tracked;
}

void QMLStockProductPusher::setTracked(bool tracked)
{
    if (m_tracked == tracked)
        return;

    m_tracked = tracked;
    emit trackedChanged();
}

bool QMLStockProductPusher::isDivisible() const
{
    return m_divisible;
}

void QMLStockProductPusher::setDivisible(bool divisible)
{
    if (m_divisible == divisible)
        return;

    m_divisible = divisible;
    emit divisibleChanged();
}

double QMLStockProductPusher::costPrice() const
{
    return m_costPrice;
}

void QMLStockProductPusher::setCostPrice(double costPrice)
{
    if (m_costPrice == costPrice)
        return;

    m_costPrice = costPrice;
    emit costPriceChanged();
}

double QMLStockProductPusher::retailPrice() const
{
    return m_retailPrice;
}

void QMLStockProductPusher::setRetailPrice(double retailPrice)
{
    if (m_retailPrice == retailPrice)
        return;

    m_retailPrice = retailPrice;
    emit retailPriceChanged();
}

void QMLStockProductPusher::push()
{
    setBusy(true);

    Utility::RecordGroup::Flags flags;
    flags.setFlag(Utility::RecordGroup::Tracked, m_tracked);
    flags.setFlag(Utility::RecordGroup::Divisible, m_divisible);

    if (isExistingProduct())
        emit execute(new StockQuery::UpdateStockProduct(
                         Utility::StockProduct {
                             m_productId,
                             Utility::StockProductCategory {
                                m_category
                             },
                             m_product,
                             m_description,
                             QUrl(),
                             Utility::StockProductUnit {
                                 m_unit,
                                 m_baseUnitEquivalent,
                             },
                             flags,
                             m_costPrice,
                             m_retailPrice,
                             Settings::DEFAULT_CURRENCY,
                             Utility::Note { m_productNote }
                         },
                         this));
    else
        emit execute(new StockQuery::AddStockProduct(Utility::StockProduct {
                                                         Utility::StockProductCategory {
                                                             m_category,
                                                             Utility::Note{ m_categoryNote }
                                                         },
                                                         m_product,
                                                         m_description,
                                                         m_imageUrl,
                                                         m_quantity,
                                                         Utility::StockProductUnit {
                                                             m_unit,
                                                             m_baseUnitEquivalent
                                                         },
                                                         flags,
                                                         m_costPrice,
                                                         m_retailPrice,
                                                         Settings::DEFAULT_CURRENCY,
                                                         Utility::Note { m_productNote }
                                                     },
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


