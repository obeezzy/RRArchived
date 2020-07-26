#ifndef TRANSACTIONEXECUTOR_H
#define TRANSACTIONEXECUTOR_H

#include "database/queryexecutor.h"

class TransactionExecutor : public QueryExecutor
{
    Q_OBJECT
public:
    explicit TransactionExecutor(const QString& command,
                                 const QVariantMap& params,
                                 const QueryRequest::QueryGroup& queryGroup,
                                 QObject* receiver);
    virtual ~TransactionExecutor() = default;

protected:
    int addDebtor(int clientId, const QString& note = QString(),
                  const QString& tableName = QString());
    int addDebtTransaction(int debtorId, int transactionId,
                           const QString& note = QString(),
                           const QString& tableName = QString());
    void addDebtPayments(int debtTransactionId, const QString& note = QString(),
                         const QString& tableName = QString());

    int addCreditor(int clientId, const QString& note = QString(),
                    const QString& tableName = QString());
    int addCreditTransaction(int creditorId, int transactionId,
                             const QString& note = QString(),
                             const QString& tableName = QString());
    void addCreditPayments(int creditTransactionId,
                           const QString& note = QString(),
                           const QString& tableName = QString());
};

#endif  // TRANSACTIONEXECUTOR_H
