#include "salecartproduct.h"
#include <QList>
#include <QVariantList>
#include <QVariantMap>
#include <QDateTime>

using namespace Utility;

SaleCartProduct::SaleCartProduct(int id) :
    id(id)
{}

SaleCartProduct::SaleCartProduct(const QString &product,
                                 const StockProductCategory &category,
                                 const QString &description,
                                 const QUrl &imageUrl,
                                 double quantity,
                                 const StockProductUnit &unit,
                                 const SaleMonies &monies,
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

SaleCartProduct::SaleCartProduct(int id,
                                 const QString &product,
                                 const StockProductCategory &category,
                                 double quantity,
                                 const StockProductUnit &unit,
                                 const SaleMonies &monies,
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

SaleCartProduct::SaleCartProduct(const QVariantMap &product) :
    id(product.value("product_id").toInt()),
    product(product.value("product").toString()),
    category(StockProductCategory{
             product.value("category_id").toInt(),
             product.value("category").toString(),
             Note{
             product.value("category_note_id").toInt(),
             product.value("category_note").toString()
             }
             }),
    description(product.value("description").toString()),
    imageUrl(product.value("image_url").toUrl()),
    quantity(product.value("quantity").toDouble()),
    availableQuantity(product.value("available_quantity").toDouble()),
    unit(StockProductUnit {
         product.value("unit_id").toInt(),
         product.value("unit").toString()
         }),
    monies(SaleMonies { QVariantMap {
            { "cost_price", product.value("cost_price").toDouble() },
            { "retail_price", product.value("retail_price").toDouble() },
            { "unit_price", product.value("unit_price").toDouble() },
            { "cost", product.value("cost").toDouble() },
            { "amount_paid", product.value("amount_paid").toDouble() }
           }}),
    note(Note {
         product.value("note_id").toInt(),
         product.value("note").toString()
         }),
    timestamp(product.value("created").toDateTime(),
              product.value("last_edited").toDateTime()),
    user(User {
         product.value("user_id").toInt(),
         product.value("user").toString()
         })
{
    flags.setFlag(RecordGroup::Tracked, product.value("tracked").toBool());
    flags.setFlag(RecordGroup::Divisible, product.value("divisible").toBool());
}

QVariantMap SaleCartProduct::toVariantMap() const
{
    return {
        { "product_id", id },
        { "product_category_id", category.id },
        { "category", category.category },
        { "quantity", quantity },
        { "product_unit_id", unit.id },
        { "unit", unit.unit },
        { "retail_price", monies.retailPrice.toDouble() },
        { "unit_price", monies.unitPrice.toDouble() },
        { "cost", monies.cost.toDouble() },
        { "amount_paid", monies.amountPaid.toDouble() },
        { "note_id", note.id },
        { "note", note.note }
    };
}
