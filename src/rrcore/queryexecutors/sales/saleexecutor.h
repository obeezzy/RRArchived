#ifndef SALEEXECUTOR_H
#define SALEEXECUTOR_H

#include "database/queryexecutor.h"

namespace SaleQuery {
class SaleExecutor : public QueryExecutor
{
    Q_OBJECT
public:
    explicit SaleExecutor(const QString &command,
                          const QVariantMap &params,
                          QObject *receiver);
    virtual ~SaleExecutor() = default;
};
}

#endif // SALEEXECUTOR_H
