#ifndef CLIENTEXECUTOR_H
#define CLIENTEXECUTOR_H

#include "database/queryexecutor.h"

class ClientExecutor : public QueryExecutor
{
    Q_OBJECT
public:
    explicit ClientExecutor(const QString &command,
                            const QVariantMap &params,
                            QObject *receiver);
    virtual ~ClientExecutor() = default;
};

#endif // CLIENTEXECUTOR_H
