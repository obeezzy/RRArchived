#ifndef VIEWINCOMEREPORT_H
#define VIEWINCOMEREPORT_H

#include "incomeexecutor.h"

namespace IncomeQuery {
class ViewIncomeReport : public IncomeExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_income_report");

    explicit ViewIncomeReport(QObject *receiver);
    QueryResult execute() override;
};
}

#endif // VIEWINCOMEREPORT_H
