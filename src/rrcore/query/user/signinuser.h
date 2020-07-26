#ifndef SIGNINUSER_H
#define SIGNINUSER_H

#include "userexecutor.h"

namespace Utility {
namespace User {
class User;
}
}  // namespace Utility

namespace Query {
namespace User {
class SignInUser : public UserExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("sign_in_user");

    explicit SignInUser(const Utility::User::User& user, QObject* receiver);
    QueryResult execute() override;

private:
    bool storeProfile(QueryResult& result, const QString& userName,
                      const QString& password);

    void attemptSignIn(QueryResult& result);
};
}  // namespace User
}  // namespace Query

Q_DECLARE_LOGGING_CATEGORY(lcsigninuser);

#endif  // SIGNINUSER_H
