#include "soldproduct.h"
#include <QVariantMap>

using namespace Utility;

SoldProduct::SoldProduct(qint64 saleTransactionId,
                         int id) :
    id(id),
    transaction(saleTransactionId)
{}

SoldProduct::SoldProduct(const QVariantMap &map)
{
    Q_UNUSED(map)
}

QVariantMap SoldProduct::toVariantMap() const
{
    return {};
}
