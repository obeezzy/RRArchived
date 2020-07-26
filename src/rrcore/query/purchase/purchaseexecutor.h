#ifndef PURCHASEEXECUTOR_H
#define PURCHASEEXECUTOR_H

#include "query/transaction/transactionexecutor.h"

class PurchaseExecutor : public TransactionExecutor
{
    Q_OBJECT
public:
    explicit PurchaseExecutor(const QString& command, const QVariantMap& params,
                              QObject* receiver);
    virtual ~PurchaseExecutor() = default;

protected:
    QueryResult addPurchaseTransaction(TransactionMode mode);

private:
    int addClient();
    int addVendor(int clientId);
    int addPurchaseTransactionToDatabase(int clientId);
    void addPurchasePayments(int purchaseTransactionId);
    void addPurchasedProducts(int purchaseTransactionId);
    void addToStockProductQuantity(const QVariantMap& product);
    void addPurchasedProduct(int purchaseTransactionId,
                             const QVariantMap& product);
};

#endif  // PURCHASEEXECUTOR_H
