#ifndef VIEWDEBTTRANSACTIONS_H
#define VIEWDEBTTRANSACTIONS_H

#include "debtorexecutor.h"

namespace Utility {
namespace Debtor {
class Debtor;
}
}  // namespace Utility

namespace Query {
namespace Debtor {
class ViewDebtTransactions : public DebtorExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND =
        QStringLiteral("view_debt_transactions");

    explicit ViewDebtTransactions(const Utility::Debtor::Debtor& debtor,
                                  QObject* receiver);
    QueryResult execute() override;

private:
    void fetchDebtor(QVariantMap& debtor);
    void fetchDebtTransactions(QVariantList& debtTransactions);
};
}  // namespace Debtor
}  // namespace Query

#endif  // VIEWDEBTTRANSACTIONS_H
