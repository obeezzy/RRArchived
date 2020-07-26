#include "stockreporttransaction.h"

using namespace Utility::Stock;

StockReportTransaction::StockReportTransaction(const QVariantMap& map)
    : category(ProductCategory{map}),
      product(Product{map}),
      openingStockQuantity(map.value("opening_stock_quantity").toDouble()),
      quantitySold(map.value("quantity_sold").toDouble()),
      quantityBought(map.value("quantity_bought").toDouble()),
      quantityInStock(map.value("quantity_in_stock").toDouble())
{}

QVariantMap StockReportTransaction::toVariantMap() const
{
    return {{"product_category_id", category.id},
            {"product_category", category.category},
            {"product_id", product.id},
            {"product", product.product},
            {"opening_stock_quantity", openingStockQuantity.toDouble()},
            {"quantity_sold", quantitySold.toDouble()},
            {"quantity_bought", quantityBought.toDouble()},
            {"quantity_in_stock", quantityInStock.toDouble()}};
}
