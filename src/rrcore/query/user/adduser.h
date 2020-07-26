#ifndef ADDUSER_H
#define ADDUSER_H

#include "userexecutor.h"

namespace Utility {
namespace User {
class User;
}
}  // namespace Utility

namespace Query {
namespace User {
class AddUser : public UserExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("add_user");

    explicit AddUser(const Utility::User::User& user, QObject* receiver);
    QueryResult execute() override;

private:
    int addRRUser();
    void addUserPrivileges(int userId);
    void addSqlUser();
};
}  // namespace User
}  // namespace Query

#endif  // ADDUSER_H
