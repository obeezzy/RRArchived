#ifndef VIEWUSERPRIVILEGES_H
#define VIEWUSERPRIVILEGES_H

#include "userexecutor.h"

namespace UserQuery {
class ViewUserPrivileges : public UserExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_user_privileges");

    explicit ViewUserPrivileges(int userId,
                                QObject *receiver);
    QueryResult execute() override;
};
}

#endif // VIEWUSERPRIVILEGES_H
