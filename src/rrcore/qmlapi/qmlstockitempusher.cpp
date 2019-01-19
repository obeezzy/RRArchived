#include "qmlstockitempusher.h"
#include "database/databasethread.h"
#include "database/databaseexception.h"

QMLStockItemPusher::QMLStockItemPusher(QObject *parent) :
    AbstractPusher(parent),
    m_itemId(-1),
    m_imageSource(QString()),
    m_category(QString()),
    m_item(QString()),
    m_description(QString()),
    m_quantity(0.0),
    m_unit(QString()),
    m_categoryNote(QString()),
    m_itemNote(QString()),
    m_tracked(false),
    m_divisible(false),
    m_costPrice(0.0),
    m_retailPrice(0.0)
{

}

QMLStockItemPusher::QMLStockItemPusher(DatabaseThread &thread) :
    AbstractPusher(thread),
    m_itemId(-1),
    m_imageSource(QString()),
    m_category(QString()),
    m_item(QString()),
    m_description(QString()),
    m_quantity(0.0),
    m_unit(QString()),
    m_categoryNote(QString()),
    m_itemNote(QString()),
    m_tracked(false),
    m_divisible(false),
    m_costPrice(0.0),
    m_retailPrice(0.0)
{

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

    QVariantMap params;
    // Don't pass quantity if you are updating the stock item.
    if (m_itemId > 0)
        params.insert("item_id", m_itemId);
    else
        params.insert("quantity", m_quantity);

    params.insert("image_source", m_imageSource);
    params.insert("category", m_category);
    params.insert("item", m_item);
    params.insert("description", m_description);
    params.insert("unit", m_unit);
    params.insert("category_note", m_categoryNote);
    params.insert("item_note", m_itemNote);
    params.insert("tracked", m_tracked);
    params.insert("divisible", m_divisible);
    params.insert("cost_price", m_costPrice);
    params.insert("retail_price", m_retailPrice);

    QueryRequest request(this);
    if (m_itemId > 0)
        request.setCommand("update_stock_item", params, QueryRequest::Stock);
    else
        request.setCommand("add_new_stock_item", params, QueryRequest::Stock);
    emit executeRequest(request);
}

void QMLStockItemPusher::processResult(const QueryResult result)
{
    if (this != result.request().receiver())
        return;

    setBusy(false);

    if (result.isSuccessful()) {
        if (result.request().command() == "add_new_stock_item")
            emit success(AddItemSuccess);
        else if (result.request().command() == "update_stock_item")
            emit success(UpdateItemSuccess);
    } else {
        switch (result.errorCode()) {
        case static_cast<int>(DatabaseException::MySqlErrorCode::UserDefinedException):
            emit error(DuplicateEntryError);
            break;
        default:
            emit error();
            break;
        }
    }
}


