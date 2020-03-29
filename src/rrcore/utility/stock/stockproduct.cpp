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
                           const StockProductQuantity &quantity,
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
{}

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
{}

StockProduct::StockProduct(int id,
                           const StockProductCategory &category,
                           const StockProductQuantity &quantity,
                           const StockProductUnit &unit,
                           const StockMonies &monies,
                           const Note &note) :
    id(id),
    category(category),
    quantity(quantity),
    unit(unit),
    monies(monies),
    note(note)
{}

StockProduct::StockProduct(const QVariantMap &map) :
    id(map.value("product_id").toInt()),
    category(StockProductCategory {
             map.value("product_category_id").toInt(),
             map.value("product_category").toString(),
             Note {
             map.value("product_category_note_id").toInt(),
             map.value("product_category_note").toString()
             }
             }),
    product(map.value("product").toString()),
    description(map.value("description").toString()),
    imageUrl(map.value("image_url").toUrl()),
    quantity(map.value("quantity").toDouble()),
    unit(StockProductUnit{ map }),
    flags((map.value("tracked").toBool() ? RecordGroup::Tracked : RecordGroup::None)
          | map.value("divisible").toBool() ? RecordGroup::Divisible : RecordGroup::None),
    monies(StockMonies{ map }),
    note(Note{ map }),
    timestamp(RecordTimestamp{ map }),
    user(User{ map })
{}

QVariantMap StockProduct::toVariantMap() const
{
    return {
        { "product_id", id },
        { "product_category_id", category.id },
        { "product_category", category.category },
        { "quantity", quantity.toDouble() },
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
