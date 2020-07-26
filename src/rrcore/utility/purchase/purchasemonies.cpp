#include "purchasemonies.h"
#include <QVariantMap>

using namespace Utility::Purchase;

PurchaseMonies::PurchaseMonies()
{}

PurchaseMonies::PurchaseMonies(const QVariantMap &map) :
    amountPaid(Money {
               map.value("amount_paid").toDouble(),
               Currency{ map.value("currency").toString() }
               }),
    balance(Money {
            map.value("balance").toDouble(),
            Currency{ map.value("currency").toString() }
            }),
    cost(Money {
         map.value("cost").toDouble(),
         Currency{ map.value("currency").toString() }
         }),
    costPrice(Money {
              map.value("cost_price").toDouble(),
              Currency{ map.value("currency").toString() }
              }),
    discount(Money {
             map.value("discount").toDouble(),
             Currency{ map.value("currency").toString() }
             }),
    retailPrice(Money {
                map.value("retail_price").toDouble(),
                Currency{ map.value("currency").toString() }
                }),
    totalCost(Money {
              map.value("total_cost").toDouble(),
              Currency{ map.value("currency").toString() }
              }),
    unitPrice(Money {
              map.value("unit_price").toDouble(),
              Currency{ map.value("currency").toString() }
              })
{}

QVariantMap PurchaseMonies::toVariantMap() const
{
    return {
        { "amount_paid", amountPaid.toDouble() },
        { "balance", balance.toDouble() },
        { "cost", cost.toDouble() },
        { "cost_price", costPrice.toDouble() },
        { "discount", discount.toDouble() },
        { "retail_price", retailPrice.toDouble() },
        { "total_cost", totalCost.toDouble() },
        { "unit_price", unitPrice.toDouble() },
        { "currency", amountPaid.currency().isoCode() }
    };
}
