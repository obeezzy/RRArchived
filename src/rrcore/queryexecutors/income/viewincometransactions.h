#ifndef VIEWINCOMETRANSACTIONS_H
#define VIEWINCOMETRANSACTIONS_H

#include "incomeexecutor.h"
#include "utility/commonutils.h"

namespace IncomeQuery {
class ViewIncomeTransactions : public IncomeExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_income_transactions");

    explicit ViewIncomeTransactions(const DateTimeSpan &dateTimeSpan,
                                    const RecordGroup::Flags &flags,
                                    QObject *receiver);
    QueryResult execute() override;
};
}

#endif // VIEWINCOMETRANSACTIONS_H
