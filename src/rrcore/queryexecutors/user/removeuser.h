#ifndef REMOVEUSER_H
#define REMOVEUSER_H

#include "userexecutor.h"

namespace UserQuery {
class RemoveUser : public UserExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("remove_user");

    explicit RemoveUser(const QString &userName,
                        QObject *receiver);
    QueryResult execute() override;
};
}

#endif // REMOVEUSER_H
