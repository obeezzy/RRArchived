#ifndef VIEWUSERDETAILS_H
#define VIEWUSERDETAILS_H

#include "userexecutor.h"

namespace UserQuery {
class ViewUserDetails : public UserExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_user_details");

    explicit ViewUserDetails(int userId,
                             QObject *receiver);
    QueryResult execute() override;
};
}

#endif // VIEWUSERDETAILS_H
