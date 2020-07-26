#ifndef FILTERCLIENTS_H
#define FILTERCLIENTS_H

#include "clientexecutor.h"
#include "utility/common/recordgroup.h"

namespace Utility {
class FilterCriteria;
}

namespace Query {
namespace Client {
class FilterClients : public ClientExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("filter_clients");

    explicit FilterClients(const Utility::FilterCriteria& filterCriteria,
                           const Utility::RecordGroup::Flags& flags,
                           QObject* receiver);
    QueryResult execute() override;
};
}  // namespace Client
}  // namespace Query

#endif  // FILTERCLIENTS_H
