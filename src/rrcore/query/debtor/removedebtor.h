#ifndef REMOVEDEBTOR_H
#define REMOVEDEBTOR_H

#include "debtorexecutor.h"

namespace Utility {
namespace Debtor {
class Debtor;
}
}  // namespace Utility

namespace Query {
namespace Debtor {
class RemoveDebtor : public DebtorExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("remove_debtor");
    static inline const QString UNDO_COMMAND =
        QStringLiteral("undo_remove_debtor");

    explicit RemoveDebtor(const Utility::Debtor::Debtor& debtor,
                          QObject* receiver);
    explicit RemoveDebtor(const QueryRequest& request, QObject* receiver);
    QueryResult execute() override;

private:
    QueryResult removeDebtor();
    QueryResult undoRemoveDebtor();

    void archiveDebtor();
    void archiveDebtTransactions();
    void fetchArchivedDebtTransactionIds(
        QVariantList& archivedDebtTransactionIds);

    // Undo
    void unarchiveDebtor();
    void unarchiveDebtTransactions(const QVariantList& debtTransactionIds);
    void fetchDebtor(QVariantMap& debtor);
};
}  // namespace Debtor
}  // namespace Query

#endif  // REMOVEDEBTOR_H
