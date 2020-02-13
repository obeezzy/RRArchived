#ifndef SIGNUPUSER_H
#define SIGNUPUSER_H

#include "userexecutor.h"

class QSqlDatabase;
namespace Utility {
class User;
}

namespace UserQuery {
class SignUpUser : public UserExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("sign_up_user");

    explicit SignUpUser(const Utility::User &user,
                        QObject *receiver);
    QueryResult execute() override;
private:
    void connectToDatabase(QSqlDatabase &connection);
    void addSqlUser(QSqlQuery &q);
};
}

#endif // SIGNUPUSER_H
