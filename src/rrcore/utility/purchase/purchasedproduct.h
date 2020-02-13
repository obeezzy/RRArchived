#ifndef PURCHASEDPRODUCT_H
#define PURCHASEDPRODUCT_H

namespace Utility {
struct PurchasedProduct
{
    int id;
    int purchaseTransactionId;

    explicit PurchasedProduct(int id,
                              int purchaseTransactionId);
};
}

#endif // PURCHASEDPRODUCT_H
