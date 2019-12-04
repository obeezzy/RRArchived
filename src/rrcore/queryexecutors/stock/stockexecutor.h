#ifndef STOCKEXECUTOR_H
#define STOCKEXECUTOR_H

#include "database/queryexecutor.h"

class StockExecutor : public QueryExecutor
{
    Q_OBJECT
public:
    explicit StockExecutor(const QString &command,
                           const QVariantMap &params,
                           QObject *receiver);
    virtual ~StockExecutor() = default;
};

#endif // STOCKEXECUTOR_H
