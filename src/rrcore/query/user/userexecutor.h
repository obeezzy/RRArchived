#ifndef USEREXECUTOR_H
#define USEREXECUTOR_H

#include "database/queryexecutor.h"

class QSqlQuery;

class UserExecutor : public QueryExecutor
{
    Q_OBJECT
public:
    explicit UserExecutor(const QString& command, const QVariantMap& params,
                          QObject* receiver);
    virtual ~UserExecutor() override = default;

protected:
    void createRRUser(const QString& userName, QSqlQuery& q);
    void grantPrivilege(const QString& privilege, const QString& userName,
                        QSqlQuery& q);
};

#endif  // USEREXECUTOR_H
