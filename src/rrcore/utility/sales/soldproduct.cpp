#include "soldproduct.h"
#include <QVariantMap>

using namespace Utility;

SoldProduct::SoldProduct(qint64 saleTransactionId) :
    transaction(saleTransactionId)
{}

SoldProduct::SoldProduct(const QVariantMap &map) :
    transaction(SaleTransaction{ map.value("sale_transaction_id").toInt() }),
    product(StockProduct {
            map.value("product_id").toInt(),
            map.value("product").toString()
            }),
    quantity(map.value("quantity").toDouble()),
    unitPrice(map.value("unit_price").toDouble()),
    cost(map.value("cost").toDouble()),
    discount(map.value("discount").toDouble()),
    note(Note {
         map.value("note_id").toInt(),
         map.value("note").toString()
         }),
    created(map.value("created").toDateTime()),
    lastEdited(map.value("last_edited").toDateTime()),
    user(User {
         map.value("user_id", -1).toInt(),
         map.value("user").toString()
         }),
    row(map.value("row", -1).toInt())
{
    product.category = StockProductCategory {
            map.value("product_category_id", -1).toInt(),
            map.value("product_category").toString()
    };
    product.unit = StockProductUnit {
            map.value("product_unit_id", -1).toInt(),
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
        { "unit_price", unitPrice },
        { "cost_price", cost },
        { "discount", discount },
        { "note_id", note.id },
        { "note", note.note },
        { "user_id", user.id },
        { "user", user.user },
        { "row", row }
    };
}
