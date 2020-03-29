#include "soldproduct.h"
#include <QVariantMap>

using namespace Utility;

SoldProduct::SoldProduct(int saleTransactionId) :
    transaction(saleTransactionId)
{}

SoldProduct::SoldProduct(const QVariantMap &map) :
    transaction(SaleTransaction{ map.value("sale_transaction_id").toInt() }),
    product(StockProduct {
            map.value("product_id").toInt(),
            map.value("product").toString()
            }),
    quantity(map.value("quantity").toDouble()),
    monies(SaleMonies { QVariantMap {
            { "unit_price", map.value("unit_price").toDouble() },
            { "cost", map.value("cost").toDouble() },
            { "discount", map.value("discount").toDouble() }
           }}),
    note(Note {
         map.value("note_id").toInt(),
         map.value("note").toString()
         }),
    timestamp(RecordTimestamp {
              map.value("created").toDateTime(),
              map.value("last_edited").toDateTime()
              }),
    user(User {
         map.value("user_id").toInt(),
         map.value("user").toString()
         }),
    row(map.value("row", -1).toInt())
{
    product.category = StockProductCategory {
            map.value("product_category_id").toInt(),
            map.value("product_category").toString()
    };
    product.unit = StockProductUnit {
            map.value("product_unit_id").toInt(),
            map.value("product_unit").toString()
    };
}

QVariantMap SoldProduct::toVariantMap() const
{
    return {
        { "sale_transaction_id", transaction.id },
        { "product_category_id", product.category.id },
        { "product_category", product.category.category },
        { "product_id", product.id },
        { "product", product.product },
        { "quantity", quantity },
        { "product_unit_id", product.unit.id },
        { "product_unit", product.unit.unit },
        { "unit_price", monies.unitPrice.toDouble() },
        { "cost_price", monies.cost.toDouble() },
        { "discount", monies.discount.toDouble() },
        { "note_id", note.id },
        { "note", note.note },
        { "user_id", user.id },
        { "user", user.user },
        { "row", row }
    };
}
