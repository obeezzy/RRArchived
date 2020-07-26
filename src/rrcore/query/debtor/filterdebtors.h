#ifndef FILTERDEBTORS_H
#define FILTERDEBTORS_H

#include "debtorexecutor.h"

namespace Utility {
class FilterCriteria;
}

namespace Query {
    namespace Debtor {
    class FilterDebtors : public DebtorExecutor
    {
        Q_OBJECT
    public:
        static inline const QString COMMAND = QStringLiteral("view_debtors");

        explicit FilterDebtors(const Utility::FilterCriteria &filterCriteria,
                               QObject *receiver);
        QueryResult execute() override;
    private:
        void filterDebtors(QVariantList &debtors);
        void filterDebtorsByName(QVariantList &debtors);
    };
}
}

#endif // FILTERDEBTORS_H
