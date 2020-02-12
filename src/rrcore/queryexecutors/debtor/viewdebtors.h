#ifndef VIEWDEBTORS_H
#define VIEWDEBTORS_H

#include "debtorexecutor.h"
#include "utility/commonutils.h"

namespace DebtorQuery {
class ViewDebtors : public DebtorExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_debtors");

    explicit ViewDebtors(QObject *receiver);
    explicit ViewDebtors(const RecordGroup::Flags &flags,
                         QObject *receiver);
    QueryResult execute() override;
};
}

#endif // VIEWDEBTORS_H
