#ifndef FILTERDEBTORS_H
#define FILTERDEBTORS_H

#include "queryexecutors/debtor/debtorexecutor.h"

class FilterCriteria;

namespace DebtorQuery {
class FilterDebtors : public DebtorExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_debtors");

    explicit FilterDebtors(const FilterCriteria &filterCriteria,
                           QObject *receiver);
    QueryResult execute() override;
private:
    void filterDebtors(QVariantList &debtors);
    void filterDebtorsByName(QVariantList &debtors);
};
}

#endif // FILTERDEBTORS_H
