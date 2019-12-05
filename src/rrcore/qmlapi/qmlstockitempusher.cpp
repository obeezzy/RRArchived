#include "qmlstockitempusher.h"
#include "database/databasethread.h"
#include "database/databaseerror.h"
#include "queryexecutors/stock.h"

#include <QUrl>

QMLStockItemPusher::QMLStockItemPusher(QObject *parent) :
    QMLStockItemPusher(DatabaseThread::instance(), parent)
{}

QMLStockItemPusher::QMLStockItemPusher(DatabaseThread &thread, QObject *parent) :
    AbstractPusher(thread, parent),
    m_itemId(-1),
    m_quantity(0.0),
    m_tracked(false),
    m_divisible(false),
    m_costPrice(0.0),
    m_retailPrice(0.0),
    m_baseUnitEquivalent(1.0)
{

}

bool QMLStockItemPusher::isExistingItem() const
{
    return m_itemId > 0;
}

int QMLStockItemPusher::itemId() const
{
    return m_itemId;
}

void QMLStockItemPusher::setItemId(int itemId)
{
    if (m_itemId == itemId)
        return;

    m_itemId = itemId;
    emit itemIdChanged();
}

QString QMLStockItemPusher::imageSource() const
{
    return m_imageSource;
}

void QMLStockItemPusher::setImageSource(const QString &imageSource)
{
    if (m_imageSource == imageSource)
        return;

    m_imageSource = imageSource;
    emit imageSourceChanged();
}

QString QMLStockItemPusher::category() const
{
    return m_category;
}

void QMLStockItemPusher::setCategory(const QString &category)
{
    if (m_category == category)
        return;

    m_category = category;
    emit categoryChanged();
}

QString QMLStockItemPusher::item() const
{
    return m_item;
}

void QMLStockItemPusher::setItem(const QString &item)
{
    if (m_item == item)
        return;

    m_item = item;
    emit itemChanged();
}

QString QMLStockItemPusher::description() const
{
    return m_description;
}

void QMLStockItemPusher::setDescription(const QString &description)
{
    if (m_description == description)
        return;

    m_description = description;
    emit descriptionChanged();
}

double QMLStockItemPusher::quantity() const
{
    return m_quantity;
}

void QMLStockItemPusher::setQuantity(double quantity)
{
    if (m_quantity == quantity)
        return;

    m_quantity = quantity;
    emit quantityChanged();
}

QString QMLStockItemPusher::unit() const
{
    return m_unit;
}

void QMLStockItemPusher::setUnit(const QString &unit)
{
    if (m_unit == unit)
        return;

    m_unit = unit;
    emit unitChanged();
}

QString QMLStockItemPusher::categoryNote() const
{
    return m_categoryNote;
}

void QMLStockItemPusher::setCategoryNote(const QString &note)
{
    if (m_categoryNote == note)
        return;

    m_categoryNote = note;
    emit categoryNoteChanged();
}

QString QMLStockItemPusher::itemNote() const
{
    return m_itemNote;
}

void QMLStockItemPusher::setItemNote(const QString &note)
{
    if (m_itemNote == note)
        return;

    m_itemNote = note;
    emit itemNoteChanged();
}

bool QMLStockItemPusher::isTracked() const
{
    return m_tracked;
}

void QMLStockItemPusher::setTracked(bool tracked)
{
    if (m_tracked == tracked)
        return;

    m_tracked = tracked;
    emit trackedChanged();
}

bool QMLStockItemPusher::isDivisible() const
{
    return m_divisible;
}

void QMLStockItemPusher::setDivisible(bool divisible)
{
    if (m_divisible == divisible)
        return;

    m_divisible = divisible;
    emit divisibleChanged();
}

double QMLStockItemPusher::costPrice() const
{
    return m_costPrice;
}

void QMLStockItemPusher::setCostPrice(double costPrice)
{
    if (m_costPrice == costPrice)
        return;

    m_costPrice = costPrice;
    emit costPriceChanged();
}

double QMLStockItemPusher::retailPrice() const
{
    return m_retailPrice;
}

void QMLStockItemPusher::setRetailPrice(double retailPrice)
{
    if (m_retailPrice == retailPrice)
        return;

    m_retailPrice = retailPrice;
    emit retailPriceChanged();
}

void QMLStockItemPusher::push()
{
    setBusy(true);

    if (isExistingItem())
        emit execute(new StockQuery::UpdateStockItem(
                         m_itemId,
                         m_category,
                         m_item,
                         m_description,
                         m_unit,
                         m_tracked,
                         m_divisible,
                         m_costPrice,
                         m_retailPrice,
                         m_baseUnitEquivalent,
                         true,
                         QStringLiteral("NGN"),
                         QUrl(),
                         QString(),
                         QString(),
                         this));
    else
        emit execute(new StockQuery::AddStockItem(
                         m_category,
                         m_item,
                         m_description,
                         m_quantity,
                         m_unit,
                         m_tracked,
                         m_divisible,
                         m_costPrice,
                         m_retailPrice,
                         m_baseUnitEquivalent,
                         true,
                         QStringLiteral("NGN"),
                         QUrl(),
                         QString(),
                         QString(),
                         this));
}

void QMLStockItemPusher::processResult(const QueryResult result)
{
    if (this != result.request().receiver())
        return;

    setBusy(false);

    if (result.isSuccessful()) {
        if (result.request().command() == StockQuery::AddStockItem::COMMAND)
            emit success(AddItemSuccess);
        else if (result.request().command() == StockQuery::UpdateStockItem::COMMAND)
            emit success(UpdateItemSuccess);
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


