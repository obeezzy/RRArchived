#ifndef CHANGEPASSWORD_H
#define CHANGEPASSWORD_H

#include "userexecutor.h"

namespace UserQuery {
class ChangePassword : public UserExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("change_password");

    explicit ChangePassword(const QString &oldPassword,
                            const QString &newPassword,
                            QObject *receiver);
    QueryResult execute() override;
};
}

#endif // CHANGEPASSWORD_H
