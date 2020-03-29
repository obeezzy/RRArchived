#include "stockproduct.h"
#include <QVariantMap>
#include <QDateTime>

using namespace Utility;

StockProduct::StockProduct(int id,
                           const QString &product,
                           const Note &note) :
    id(id),
    product(product),
    note(note)
{}

StockProduct::StockProduct(const StockProductCategory &category,
                           const QString &product,
                           const QString &description,
                           const QUrl &imageUrl,
                           double quantity,
                           const StockProductUnit &unit,
                           const RecordGroup::Flags &flags,
                           const StockMonies &monies,
                           const Note &note) :
    category(category),
    product(product),
    description(description),
    imageUrl(imageUrl),
    quantity(quantity),
    unit(unit),
    flags(flags),
    monies(monies),
    note(note)
{ }

StockProduct::StockProduct(int id,
                           const StockProductCategory &category,
                           const QString &product,
                           const QString &description,
                           const QUrl &imageUrl,
                           const StockProductUnit &unit,
                           const RecordGroup::Flags &flags,
                           const StockMonies &monies,
                           const Note &note) :
    id(id),
    category(category),
    product(product),
    description(description),
    imageUrl(imageUrl),
    unit(unit),
    flags(flags),
    monies(monies),
    note(note)
{ }

StockProduct::StockProduct(int id,
                           const StockProductCategory &category,
                           double quantity,
                           const StockProductUnit &unit,
                           const StockMonies &monies,
                           const Note &note) :
    id(id),
    category(category),
    quantity(quantity),
    unit(unit),
    monies(monies),
    note(note)
{ }

StockProduct::StockProduct(const QVariantMap &product) :
    id(product.value("product_id").toInt()),
    category(StockProductCategory{
             product.value("product_category_id").toInt(),
             product.value("product_category").toString(),
             Note{
             product.value("product_category_note_id").toInt(),
             product.value("product_category_note").toString()
             }
             }),
    product(product.value("product").toString()),
    description(product.value("description").toString()),
    imageUrl(product.value("image_url").toUrl()),
    quantity(product.value("quantity").toDouble()),
    unit(StockProductUnit {
         product.value("product_unit_id").toInt(),
         product.value("product_unit").toString()
         }),
    monies(StockMonies { QVariantMap {
            { "cost_price", product.value("cost_price").toDouble() },
            { "retail_price", product.value("retail_price").toDouble() },
            { "unit_price", product.value("unit_price").toDouble() }
           }}),
    note(Note {product.value("note_id").toInt(),
         product.value("note").toString()
         }),
    timestamp(RecordTimestamp {
              product.value("created").toDateTime(),
              product.value("last_edited").toDateTime()
              }),
    user(User{
         product.value("user_id").toInt(),
         product.value("user").toString()
         })
{
    flags.setFlag(RecordGroup::Tracked, product.value("tracked").toBool());
    flags.setFlag(RecordGroup::Divisible, product.value("divisible").toBool());
}

QVariantMap StockProduct::toVariantMap() const
{
    return {
        { "product_id", id },
        { "product_category_id", category.id },
        { "product_category", category.category },
        { "quantity", quantity },
        { "product_unit_id", unit.id },
        { "product_unit", unit.unit },
        { "cost_price", monies.costPrice.toDouble() },
        { "retail_price", monies.retailPrice.toDouble() },
        { "unit_price", monies.unitPrice.toDouble() },
        { "divisible", flags.testFlag(RecordGroup::Divisible) },
        { "note_id", note.id },
        { "note", note.note }
    };
}
