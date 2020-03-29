#include "purchasereporttransaction.h"
#include "utility/stock/stockproductcategory.h"
#include "utility/stock/stockproduct.h"

using namespace Utility;

PurchaseReportTransaction::PurchaseReportTransaction(const QVariantMap &map) :
    category(StockProductCategory{ map }),
    product(StockProduct{ map }),
    quantityBought(map.value("quantity_bought").toDouble()),
    totalExpenditure(map.value("total_expenditure").toDouble())
{
}

QVariantMap PurchaseReportTransaction::toVariantMap() const
{
    return {
        { "product_category_id", category.id },
        { "product_category", category.category },
        { "product_id", product.id },
        { "product", product.product },
        { "quantity_bought", quantityBought.toDouble() },
        { "total_expenditure", totalExpenditure.toDouble() }
    };
}
