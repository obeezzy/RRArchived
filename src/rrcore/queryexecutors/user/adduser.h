#ifndef ADDUSER_H
#define ADDUSER_H

#include "userexecutor.h"

namespace UserQuery {
class AddUser : public UserExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("add_user");

    explicit AddUser(const QString &firstName,
                     const QString &lastName,
                     const QString &userName,
                     const QString &password,
                     const QString &phoneNumber,
                     const QString &emailAddress,
                     const QUrl &imageUrl,
                     QObject *receiver);
    QueryResult execute() override;
};
}

#endif // ADDUSER_H
