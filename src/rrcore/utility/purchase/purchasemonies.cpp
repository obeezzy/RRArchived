#include "purchasemonies.h"
#include <QVariantMap>

using namespace Utility;

PurchaseMonies::PurchaseMonies()
{}

PurchaseMonies::PurchaseMonies(const QVariantMap &map) :
    amountPaid(Money {
               map.value("amount_paid").toDouble(),
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
    retailPrice(Money {
                map.value("retail_price").toDouble(),
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
        { "cost_price", costPrice.toDouble() },
        { "retail_price", retailPrice.toDouble() },
        { "unit_price", unitPrice.toDouble() },
        { "cost", cost.toDouble() },
        { "amount_paid", amountPaid.toDouble() },
        { "currency", amountPaid.currency().isoCode() }
    };
}
