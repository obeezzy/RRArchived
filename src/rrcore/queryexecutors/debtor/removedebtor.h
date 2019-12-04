#ifndef REMOVEDEBTOR_H
#define REMOVEDEBTOR_H

#include "debtorexecutor.h"

namespace DebtorQuery {
class RemoveDebtor : public DebtorExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("remove_debtor");
    static inline const QString UNDO_COMMAND = QStringLiteral("undo_remove_debtor");

    explicit RemoveDebtor(int debtorId,
                          int debtorRow,
                          QObject *receiver);
    QueryResult execute() override;
private:
    QueryResult removeDebtor();
    QueryResult undoRemoveDebtor();
};
}

#endif // REMOVEDEBTOR_H
