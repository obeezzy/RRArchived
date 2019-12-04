#ifndef PURCHASEEXECUTOR_H
#define PURCHASEEXECUTOR_H

#include "database/queryexecutor.h"

class PurchaseExecutor : public QueryExecutor
{
    Q_OBJECT
public:
    explicit PurchaseExecutor(const QString &command,
                              const QVariantMap &params,
                              QObject *receiver);
    virtual ~PurchaseExecutor() = default;
protected:
    QueryResult addPurchaseTransaction(TransactionMode mode);
};

#endif // PURCHASEEXECUTOR_H
