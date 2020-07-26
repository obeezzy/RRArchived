#ifndef VIEWINCOMEREPORT_H
#define VIEWINCOMEREPORT_H

#include "incomeexecutor.h"

namespace Utility {
class DateTimeSpan;
}

namespace Query {
    namespace Income {
    class ViewIncomeReport : public IncomeExecutor
    {
        Q_OBJECT
    public:
        static inline const QString COMMAND = QStringLiteral("view_income_report");

        explicit ViewIncomeReport(const Utility::DateTimeSpan &dateTimeSpan,
                                  QObject *receiver);
        QueryResult execute() override;
    };
}
}

#endif // VIEWINCOMEREPORT_H
