#ifndef FILTERCLIENTS_H
#define FILTERCLIENTS_H

#include "queryexecutors/client/clientexecutor.h"
#include "utility/commonutils.h"

namespace ClientQuery {
class FilterClients : public ClientExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("filter_clients");

    explicit FilterClients(const Utility::FilterCriteria &filterCriteria,
                           const Utility::RecordGroup::Flags &flags,
                           QObject *receiver);
    QueryResult execute() override;
};
}

#endif // FILTERCLIENTS_H
