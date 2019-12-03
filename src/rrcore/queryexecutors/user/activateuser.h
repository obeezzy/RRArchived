#ifndef ACTIVATEUSER_H
#define ACTIVATEUSER_H

#include "userexecutor.h"

namespace UserQuery {
class ActivateUser : public UserExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("activate_user");

    explicit ActivateUser(const QString &userName,
                          bool active,
                          QObject *receiver);
    QueryResult execute() override;
};
}

#endif // ACTIVATEUSER_H
