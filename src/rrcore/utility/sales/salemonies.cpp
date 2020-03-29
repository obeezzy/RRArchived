#include "salemonies.h"

using namespace Utility;

SaleMonies::SaleMonies()
{}

SaleMonies::SaleMonies(const QVariantMap &map) :
    amountPaid(map.value("amount_paid").toDouble()),
    balance(map.value("balance").toDouble()),
    cost(map.value("cost").toDouble()),
    costPrice(map.value("cost_price").toDouble()),
    discount(map.value("discount").toDouble()),
    retailPrice(map.value("retail_price").toDouble()),
    totalCost(map.value("total_cost").toDouble()),
    unitPrice(map.value("unit_price").toDouble())
{}

QVariantMap SaleMonies::toVariantMap() const
{
    return {
        { "amount_paid", amountPaid.toDouble() },
        { "balance", balance.toDouble() },
        { "cost", cost.toDouble() },
        { "cost_price", costPrice.toDouble() },
        { "discount", discount.toDouble() },
        { "retail_price", retailPrice.toDouble() },
        { "total_cost", totalCost.toDouble() },
        { "unit_price", unitPrice.toDouble() }
    };
}
