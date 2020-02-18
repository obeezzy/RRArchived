#ifndef ADDUSER_H
#define ADDUSER_H

#include "userexecutor.h"

namespace Utility {
class User;
}

namespace UserQuery {
class AddUser : public UserExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("add_user");

    explicit AddUser(const Utility::User &user,
                     QObject *receiver);
    QueryResult execute() override;
private:
    int addRRUser();
    void addUserPrivileges(int userId);
    void addSqlUser();
};
}

#endif // ADDUSER_H
