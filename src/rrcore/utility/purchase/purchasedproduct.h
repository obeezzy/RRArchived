#ifndef PURCHASEDPRODUCT_H
#define PURCHASEDPRODUCT_H

struct PurchasedProduct
{
    int id;
    int purchaseTransactionId;

    explicit PurchasedProduct(int id,
                              int purchaseTransactionId);
};

#endif // PURCHASEDPRODUCT_H
