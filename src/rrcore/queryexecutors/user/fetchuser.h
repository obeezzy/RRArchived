#ifndef FETCHUSER_H
#define FETCHUSER_H

#include "userexecutor.h"

namespace UserQuery {
class FetchUser : public UserExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("fetch_user");

    explicit FetchUser(int userId,
                       QObject *receiver);
    QueryResult execute() override;
};
}

#endif // FETCHUSER_H
