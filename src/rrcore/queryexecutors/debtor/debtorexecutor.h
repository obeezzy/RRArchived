#ifndef DEBTOREXECUTOR_H
#define DEBTOREXECUTOR_H

#include "database/queryexecutor.h"

class DebtorExecutor : public QueryExecutor
{
    Q_OBJECT
public:
    explicit DebtorExecutor(const QString &command,
                            const QVariantMap &params,
                            QObject *receiver);
    virtual ~DebtorExecutor() = default;
};

#endif // DEBTOREXECUTOR_H
