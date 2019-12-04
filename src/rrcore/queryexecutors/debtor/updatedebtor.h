#ifndef UPDATEDEBTOR_H
#define UPDATEDEBTOR_H

#include "debtorexecutor.h"
#include "utility/debtorutils.h"

#include <QUrl>

namespace DebtorQuery {
class UpdateDebtor : public DebtorExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("update_debtor");

    explicit UpdateDebtor(int debtorId,
                          int clientId,
                          const QString &preferredName,
                          const QString &firstName,
                          const QString &lastName,
                          const QString &primaryPhoneNumber,
                          const QUrl &imageUrl,
                          const DebtTransactionList &newDebtTransactions,
                          const DebtTransactionList &updatedDebtTransactions,
                          const QString &note,
                          QObject *receiver);
    QueryResult execute() override;
};
}

#endif // UPDATEDEBTOR_H
