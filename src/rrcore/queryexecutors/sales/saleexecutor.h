#ifndef SALEEXECUTOR_H
#define SALEEXECUTOR_H

#include "database/queryexecutor.h"

class SaleExecutor : public QueryExecutor
{
    Q_OBJECT
public:
    explicit SaleExecutor(const QString &command,
                          const QVariantMap &params,
                          QObject *receiver);
    virtual ~SaleExecutor() = default;
protected:
    QueryResult addSaleTransaction(TransactionMode mode);
};

#endif // SALEEXECUTOR_H
