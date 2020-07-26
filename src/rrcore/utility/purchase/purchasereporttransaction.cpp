#include "purchasereporttransaction.h"
#include "utility/stock/product.h"
#include "utility/stock/productcategory.h"

using namespace Utility::Purchase;

PurchaseReportTransaction::PurchaseReportTransaction(const QVariantMap& map)
    : category(Stock::ProductCategory{map}),
      product(Stock::Product{map}),
      quantityBought(map.value("quantity_bought").toDouble()),
      totalExpenditure(map.value("total_expenditure").toDouble())
{}

QVariantMap PurchaseReportTransaction::toVariantMap() const
{
    return {{"product_category_id", category.id},
            {"product_category", category.category},
            {"product_id", product.id},
            {"product", product.product},
            {"quantity_bought", quantityBought.toDouble()},
            {"total_expenditure", totalExpenditure.toDouble()}};
}
