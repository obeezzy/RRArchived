#include "stockproduct.h"
#include <QList>
#include <QVariantList>
#include <QVariantMap>
#include <QDateTime>

StockProduct::StockProduct(int id) :
    id(id),
    row(-1)
{

}

StockProduct::StockProduct(const StockProductCategory &category,
                           const QString &product,
                           const QString &description,
                           const QUrl &imageUrl,
                           double quantity,
                           const StockProductUnit &unit,
                           bool tracked,
                           bool divisible,
                           qreal costPrice,
                           qreal retailPrice,
                           const QString &currency,
                           const Note &note) :
    id(-1),
    category(category),
    product(product),
    description(description),
    imageUrl(imageUrl),
    quantity(quantity),
    unit(unit),
    tracked(tracked),
    divisible(divisible),
    costPrice(costPrice),
    retailPrice(retailPrice),
    currency(currency),
    note(note),
    row(-1)
{ }

StockProduct::StockProduct(int id,
                           const StockProductCategory &category,
                           const QString &product,
                           const QString &description,
                           const QUrl &imageUrl,
                           const StockProductUnit &unit,
                           bool tracked,
                           bool divisible,
                           qreal costPrice,
                           qreal retailPrice,
                           const QString &currency,
                           const Note &note) :
    id(id),
    category(category),
    product(product),
    description(description),
    imageUrl(imageUrl),
    quantity(0.0),
    unit(unit),
    tracked(tracked),
    divisible(divisible),
    costPrice(costPrice),
    retailPrice(retailPrice),
    currency(currency),
    note(note),
    row(-1)
{ }

StockProduct::StockProduct(int id,
                           const StockProductCategory &category,
                           double quantity,
                           const StockProductUnit &unit,
                           qreal retailPrice,
                           qreal unitPrice,
                           const Note &note) :
    id(id),
    category(category),
    quantity(quantity),
    unit(unit),
    tracked(true),
    divisible(true),
    retailPrice(retailPrice),
    unitPrice(unitPrice),
    note(note),
    row(-1)
{ }

StockProduct::StockProduct(const QVariantMap &product) :
    id(product.value("product_id").toInt()),
    category(StockProductCategory{
             product.value("category_id").toInt(),
             product.value("category").toString(),
             Note{
             product.value("category_note_id").toInt(),
             product.value("category_note").toString()
             }
             }),
    product(product.value("product").toString()),
    description(product.value("description").toString()),
    imageUrl(product.value("image_url").toUrl()),
    quantity(product.value("quantity").toDouble()),
    unit(StockProductUnit {
         product.value("unit_id").toInt(),
         product.value("unit").toString()
         }),
    tracked(product.value("tracked").toBool()),
    divisible(product.value("divisible").toBool()),
    costPrice(product.value("cost_price").toDouble()),
    retailPrice(product.value("retail_price").toDouble()),
    unitPrice(product.value("unit_price").toDouble()),
    currency(product.value("currency").toString()),
    note(Note {product.value("note_id").toInt(),
         product.value("note").toString()
         }),
    created(product.value("created").toDateTime()),
    lastEdited(product.value("last_edited").toDateTime()),
    user(User{
         product.value("user_id").toInt(),
         product.value("user").toString()
         }),
    row(-1)
{}

QVariantMap StockProduct::toVariantMap() const
{
    return {
        { "product_id", id },
        { "product_category_id", category.id },
        { "category", category.category },
        { "quantity", quantity },
        { "product_unit_id", unit.id },
        { "unit", unit.unit },
        { "cost_price", costPrice },
        { "retail_price", retailPrice },
        { "unit_price", unitPrice },
        { "note_id", note.id },
        { "note", note.note }
    };
}
