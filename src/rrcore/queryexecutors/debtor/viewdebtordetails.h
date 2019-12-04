#ifndef VIEWDEBTORDETAILS_H
#define VIEWDEBTORDETAILS_H

#include "debtorexecutor.h"

namespace DebtorQuery {
class ViewDebtorDetails : public DebtorExecutor
{
public:
    static inline const QString COMMAND = QStringLiteral("view_debtor_details");

    explicit ViewDebtorDetails(int debtorId,
                               QObject *receiver);
    QueryResult execute() override;
};
}

#endif // VIEWDEBTORDETAILS_H
