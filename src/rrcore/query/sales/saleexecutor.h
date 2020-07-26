#ifndef SALEEXECUTOR_H
#define SALEEXECUTOR_H

#include "query/transaction/transactionexecutor.h"

class SaleExecutor : public TransactionExecutor
{
    Q_OBJECT
public:
    explicit SaleExecutor(const QString &command,
                          const QVariantMap &params,
                          QObject *receiver);
    virtual ~SaleExecutor() = default;
protected:
    QueryResult addSaleTransaction(TransactionMode mode);

    int addClient();
    int addCustomer(int clientId);
    int addSaleTransactionToDatabase(int clientId);
    void addSalePayments(int saleTransactionId);
    void addSoldProducts(int saleTransactionId);

    void deductFromStockProductQuantity(const QVariantMap &product);
};

#endif // SALEEXECUTOR_H
