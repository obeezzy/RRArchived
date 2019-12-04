#ifndef INCOMEEXECUTOR_H
#define INCOMEEXECUTOR_H

#include "database/queryexecutor.h"

class IncomeExecutor : public QueryExecutor
{
    Q_OBJECT
public:
    explicit IncomeExecutor(const QString &command,
                            const QVariantMap &params,
                            QObject *receiver);
    virtual ~IncomeExecutor() = default;
};

#endif // INCOMEEXECUTOR_H
