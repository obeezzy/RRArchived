#ifndef VIEWCLIENTS_H
#define VIEWCLIENTS_H

#include "clientexecutor.h"

namespace ClientQuery {
class ViewClients : public ClientExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_clients");

    explicit ViewClients(QObject *receiver);
    explicit ViewClients(const QString &filterText,
                         const QString &filterColumn,
                         QObject *receiver);
    QueryResult execute() override;
};
}

#endif // VIEWCLIENTS_H
