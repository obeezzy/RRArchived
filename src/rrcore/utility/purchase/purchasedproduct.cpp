#include "purchasedproduct.h"

using namespace Utility;

PurchasedProduct::PurchasedProduct(int id,
                                   int purchaseTransactionId) :
    id(id)
{
    Q_UNUSED(purchaseTransactionId)
}

PurchasedProduct::PurchasedProduct(const QVariantMap &map)
{
    Q_UNUSED(map)
}
