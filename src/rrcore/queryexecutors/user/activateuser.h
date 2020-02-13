#ifndef ACTIVATEUSER_H
#define ACTIVATEUSER_H

#include "userexecutor.h"

namespace Utility {
class User;
}
namespace UserQuery {
class ActivateUser : public UserExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("activate_user");

    explicit ActivateUser(bool active,
                          const Utility::User &user,
                          QObject *receiver);
    QueryResult execute() override;
};
}

#endif // ACTIVATEUSER_H
