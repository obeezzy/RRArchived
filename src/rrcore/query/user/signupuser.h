#ifndef SIGNUPUSER_H
#define SIGNUPUSER_H

#include "userexecutor.h"

class QSqlDatabase;
namespace Utility {
namespace User {
class User;
}
}  // namespace Utility

namespace Query {
namespace User {
class SignUpUser : public UserExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("sign_up_user");

    explicit SignUpUser(const Utility::User::User& user, QObject* receiver);
    QueryResult execute() override;

private:
    void connectToDatabase(QSqlDatabase& connection);
    void addSqlUser(QSqlQuery& q);
};
}  // namespace User
}  // namespace Query

#endif  // SIGNUPUSER_H
