#ifndef ADDDEBTOR_H
#define ADDDEBTOR_H

#include "debtorexecutor.h"
#include "utility/debtorutils.h"

#include <QUrl>

namespace DebtorQuery {
class AddDebtor : public DebtorExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("add_new_debtor");
    static inline const QString UNDO_COMMAND = QStringLiteral("undo_add_new_debtor");

    explicit AddDebtor(const QString &preferredName,
                       const QString &firstName,
                       const QString &lastName,
                       const QString &primaryPhoneNumber,
                       const QUrl &imageUrl,
                       const DebtTransactionList &newDebtTransactions,
                       const QString &note,
                       QObject *receiver);
    QueryResult execute() override;
private:
    QueryResult addDebtor();
    QueryResult undoAddDebtor();
};
}

#endif // ADDDEBTOR_H
