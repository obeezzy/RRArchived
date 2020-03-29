#include "purchasecartproduct.h"
#include <QDateTime>
#include <QVariantList>

using namespace Utility;

PurchaseCartProduct::PurchaseCartProduct(const QString &product,
                                         const StockProductCategory &category,
                                         const QString &description,
                                         const QUrl &imageUrl,
                                         const StockProductQuantity &quantity,
                                         const StockProductUnit &unit,
                                         const PurchaseMonies &monies,
                                         const RecordGroup::Flags &flags,
                                         const Note &note) :
    product(product),
    category(category),
    description(description),
    imageUrl(imageUrl),
    quantity(quantity),
    unit(unit),
    monies(monies),
    flags(flags),
    note(note)
{}

PurchaseCartProduct::PurchaseCartProduct(int id,
                                         const QString &product,
                                         const StockProductCategory &category,
                                         const StockProductQuantity &quantity,
                                         const StockProductUnit &unit,
                                         const PurchaseMonies &monies,
                                         const Note &note) :
    id(id),
    product(product),
    category(category),
    quantity(quantity),
    unit(unit),
    monies(monies),
    flags(RecordGroup::Tracked | RecordGroup::Divisible),
    note(note)
{}

PurchaseCartProduct::PurchaseCartProduct(int id) :
    id(id)
{}

PurchaseCartProduct::PurchaseCartProduct(const QVariantMap &map) :
    id(map.value("product_id").toInt()),
    product(map.value("product").toString()),
    category(StockProductCategory{
             map.value("category_id").toInt(),
             map.value("category").toString(),
             Note{
             map.value("category_note_id").toInt(),
             map.value("category_note").toString()
             }
             }),
    description(map.value("description").toString()),
    imageUrl(map.value("image_url").toUrl()),
    quantity(map.value("quantity").toDouble()),
    availableQuantity(map.value("available_quantity").toDouble()),
    unit(StockProductUnit{ map }),
    monies(PurchaseMonies{ map }),
    flags((map.value("tracked").toBool() ? RecordGroup::Tracked : RecordGroup::None)
          | (map.value("divisible").toBool() ? RecordGroup::Divisible : RecordGroup::None)),
    note(Note{ map }),
    timestamp(RecordTimestamp{ map }),
    user(User{ map })
{}

QVariantMap PurchaseCartProduct::toVariantMap() const
{
    return {
        { "product_id", id },
        { "product_category_id", category.id },
        { "category", category.category },
        { "quantity", quantity.toDouble() },
        { "available_quantity", availableQuantity.toDouble() },
        { "product_unit_id", unit.id },
        { "unit", unit.unit },
        { "retail_price", monies.retailPrice.toDouble() },
        { "unit_price", monies.unitPrice.toDouble() },
        { "cost", monies.cost.toDouble() },
        { "amount_paid", monies.amountPaid.toDouble() },
        { "currency", monies.retailPrice.currency().isoCode() },
        { "note_id", note.id },
        { "note", note.note }
    };
}
