#ifndef ACTIVATEUSER_H
#define ACTIVATEUSER_H

#include "userexecutor.h"
#include "utility/userutils.h"

namespace UserQuery {
class ActivateUser : public UserExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("activate_user");

    explicit ActivateUser(bool active,
                          const User &user,
                          QObject *receiver);
    QueryResult execute() override;
};
}

#endif // ACTIVATEUSER_H
