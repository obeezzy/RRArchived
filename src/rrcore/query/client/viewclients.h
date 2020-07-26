#ifndef VIEWCLIENTS_H
#define VIEWCLIENTS_H

#include "clientexecutor.h"
#include "utility/common/recordgroup.h"

namespace Query {
    namespace Client {
    class ViewClients : public ClientExecutor
    {
        Q_OBJECT
    public:
        static inline const QString COMMAND = QStringLiteral("view_clients");

        explicit ViewClients(QObject *receiver);
        explicit ViewClients(const Utility::RecordGroup::Flags &flags,
                             QObject *receiver);
        QueryResult execute() override;
    };
}
}

#endif // VIEWCLIENTS_H
