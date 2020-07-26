#ifndef EXPENSEEXECUTOR_H
#define EXPENSEEXECUTOR_H

#include "database/queryexecutor.h"

class ExpenseExecutor : public QueryExecutor
{
    Q_OBJECT
public:
    explicit ExpenseExecutor(const QString &command,
                             const QVariantMap &params,
                             QObject *receiver);
    virtual ~ExpenseExecutor() = default;
};

#endif // EXPENSEEXECUTOR_H
