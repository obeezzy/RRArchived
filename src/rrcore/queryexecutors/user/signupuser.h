#ifndef SIGNUPUSER_H
#define SIGNUPUSER_H

#include "userexecutor.h"

namespace UserQuery {
class SignUpUser : public UserExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("sign_up_user");

    explicit SignUpUser(const QString &userName, const QString &password,
                        QObject *receiver);
    QueryResult execute() override;
};
}

#endif // SIGNUPUSER_H
