#include "salecartproduct.h"
#include <QList>
#include <QVariantList>
#include <QVariantMap>
#include <QDateTime>

using namespace Utility;

SaleCartProduct::SaleCartProduct(int id) :
    id(id)
{}

SaleCartProduct::SaleCartProduct(const StockProductCategory &category,
                                 const QString &product,
                                 const QString &description,
                                 const QUrl &imageUrl,
                                 qreal quantity,
                                 const StockProductUnit &unit,
                                 const RecordGroup::Flags &flags,
                                 qreal costPrice,
                                 qreal retailPrice,
                                 const QString &currency,
                                 const Note &note) :
    product(product),
    category(category),
    description(description),
    imageUrl(imageUrl),
    quantity(quantity),
    unit(unit),
    flags(flags),
    costPrice(costPrice),
    retailPrice(retailPrice),
    currency(currency),
    note(note)
{}

SaleCartProduct::SaleCartProduct(int id,
                                 const QString &product,
                                 const StockProductCategory &category,
                                 qreal quantity,
                                 const StockProductUnit &unit,
                                 qreal retailPrice,
                                 qreal unitPrice,
                                 qreal cost,
                                 qreal amountPaid,
                                 const Note &note) :
    id(id),
    product(product),
    category(category),
    quantity(quantity),
    unit(unit),
    flags(RecordGroup::Tracked | RecordGroup::Divisible),
    retailPrice(retailPrice),
    unitPrice(unitPrice),
    cost(cost),
    amountPaid(amountPaid),
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
    unit(StockProductUnit {
         product.value("unit_id").toInt(),
         product.value("unit").toString()
         }),
    costPrice(product.value("cost_price").toDouble()),
    retailPrice(product.value("retail_price").toDouble()),
    unitPrice(product.value("unit_price").toDouble()),
    cost(product.value("cost").toDouble()),
    amountPaid(product.value("amount_paid").toDouble()),
    currency(product.value("currency").toString()),
    note(Note {
         product.value("note_id").toInt(),
         product.value("note").toString()
         }),
    created(product.value("created").toDateTime()),
    lastEdited(product.value("last_edited").toDateTime()),
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
        { "retail_price", retailPrice },
        { "unit_price", unitPrice },
        { "cost", cost },
        { "amount_paid", amountPaid },
        { "note_id", note.id },
        { "note", note.note }
    };
}
