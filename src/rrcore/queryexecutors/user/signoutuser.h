#ifndef SIGNOUTUSER_H
#define SIGNOUTUSER_H

#include "userexecutor.h"

namespace UserQuery {
class SignOutUser : public UserExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("sign_out_user");

    explicit SignOutUser(QObject *receiver);
    QueryResult execute() override;
};
}

#endif // SIGNOUTUSER_H
