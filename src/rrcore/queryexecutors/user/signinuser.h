#ifndef SIGNINUSER_H
#define SIGNINUSER_H

#include "userexecutor.h"

namespace UserQuery {
class SignInUser : public UserExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("sign_in_user");

    explicit SignInUser(const QString &userName,
                        const QString &password,
                        QObject *receiver);
    QueryResult execute() override;
private:
    bool storeProfile(QueryResult &result, const QString &userName, const QString &password);
};
}

Q_DECLARE_LOGGING_CATEGORY(signInUser);

#endif // SIGNINUSER_H
