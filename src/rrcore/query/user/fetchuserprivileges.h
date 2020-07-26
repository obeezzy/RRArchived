#ifndef FETCHUSERPRIVILEGES_H
#define FETCHUSERPRIVILEGES_H

#include "userexecutor.h"

namespace Query {
namespace User {
class FetchUserPrivileges : public UserExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND =
        QStringLiteral("fetch_user_privileges");

    explicit FetchUserPrivileges(int userId, QObject* receiver);
    QueryResult execute() override;
};
}  // namespace User
}  // namespace Query

#endif  // FETCHUSERPRIVILEGES_H
