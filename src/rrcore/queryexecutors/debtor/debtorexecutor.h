#ifndef DEBTOREXECUTOR_H
#define DEBTOREXECUTOR_H

#include "database/queryexecutor.h"

namespace Utility {
class DebtTransactionList;
}

class DebtorExecutor : public QueryExecutor
{
    Q_OBJECT
public:
    explicit DebtorExecutor(const QString &command,
                            const QVariantMap &params,
                            QObject *receiver);
    virtual ~DebtorExecutor() = default;
protected:
    void arrangeDebtTransactions(const Utility::DebtTransactionList &debtTransactions);
};

#endif // DEBTOREXECUTOR_H
