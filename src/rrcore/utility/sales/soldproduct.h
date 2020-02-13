#ifndef SOLDPRODUCT_H
#define SOLDPRODUCT_H

#include <QObject>

namespace Utility {
struct SoldProduct
{
    qint64 saleTransactionId;
    int id;

    explicit SoldProduct(qint64 saleTransactionId,
                         int id);
};
}

#endif // SOLDPRODUCT_H
