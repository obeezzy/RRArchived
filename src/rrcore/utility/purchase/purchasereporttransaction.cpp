#include "purchasereporttransaction.h"
#include "utility/stock/stockproductcategory.h"
#include "utility/stock/stockproduct.h"

using namespace Utility;

PurchaseReportTransaction::PurchaseReportTransaction(const QVariantMap &map) :
    quantityBought(0.0),
    totalAmount(0.0)
{
    Q_UNUSED(map)
}

QVariantMap PurchaseReportTransaction::toVariantMap() const
{
    return { };
}
