#ifndef REMOVEUSER_H
#define REMOVEUSER_H

#include "userexecutor.h"

namespace Utility {
class User;
}

namespace UserQuery {
class RemoveUser : public UserExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("remove_user");
    static inline const QString UNDO_COMMAND = QStringLiteral("undo_remove_user");

    explicit RemoveUser(const Utility::User &user,
                        QObject *receiver);
    QueryResult execute() override;
};
}

#endif // REMOVEUSER_H
