#include "salereporttransaction.h"

using namespace Utility::Sales;

SaleReportTransaction::SaleReportTransaction(const QVariantMap &map) :
    category(Stock::ProductCategory{ map }),
    product(Stock::Product{ map }),
    quantitySold(map.value("quantity_sold").toDouble()),
    totalRevenue(map.value("total_revenue").toDouble())
{}

QVariantMap SaleReportTransaction::toVariantMap() const
{
    return {
        { "product_category_id", category.id },
        { "product_category", category.category },
        { "product_id", product.id },
        { "product", product.product },
        { "quantity_sold", quantitySold.toDouble() },
        { "total_revenue", totalRevenue.toDouble() }
    };
}
