#ifndef VIEWUSERS_H
#define VIEWUSERS_H

#include "userexecutor.h"

namespace UserQuery {
class ViewUsers : public UserExecutor
{
    Q_OBJECT
public:
    static inline const  QString COMMAND = QStringLiteral("view_users");

    explicit ViewUsers(bool archived,
                       QObject *receiver);
    QueryResult execute() override;
};
}

#endif // VIEWUSERS_H
