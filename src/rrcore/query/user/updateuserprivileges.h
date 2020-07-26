#ifndef UPDATEUSERPRIVILEGES_H
#define UPDATEUSERPRIVILEGES_H

#include "userexecutor.h"

namespace Utility {
namespace User {
class UserPrivilegeList;
}
}

namespace Query {
    namespace User {
    class UpdateUserPrivileges : public UserExecutor
    {
        Q_OBJECT
    public:
        static inline const QString COMMAND = QStringLiteral("update_user_privileges");

        explicit UpdateUserPrivileges(int userId,
                                      const Utility::User::UserPrivilegeList &privilege,
                                      QObject *receiver);
        QueryResult execute() override;
    };
}
}

#endif // UPDATEUSERPRIVILEGES_H
