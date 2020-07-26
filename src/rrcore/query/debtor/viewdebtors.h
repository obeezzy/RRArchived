#ifndef VIEWDEBTORS_H
#define VIEWDEBTORS_H

#include "debtorexecutor.h"
#include "utility/common/recordgroup.h"

namespace Query {
namespace Debtor {
class ViewDebtors : public DebtorExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_debtors");

    explicit ViewDebtors(QObject* receiver);
    explicit ViewDebtors(const Utility::RecordGroup::Flags& flags,
                         QObject* receiver);
    QueryResult execute() override;
};
}  // namespace Debtor
}  // namespace Query

#endif  // VIEWDEBTORS_H
