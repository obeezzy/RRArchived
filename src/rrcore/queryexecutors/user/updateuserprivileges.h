#ifndef UPDATEUSERPRIVILEGES_H
#define UPDATEUSERPRIVILEGES_H

#include "userexecutor.h"

namespace UserQuery {
class UpdateUserPrivileges : public UserExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("update_user_privileges");

    explicit UpdateUserPrivileges(int userId,
                                  QObject *receiver);
    QueryResult execute() override;
};
}

#endif // UPDATEUSERPRIVILEGES_H
