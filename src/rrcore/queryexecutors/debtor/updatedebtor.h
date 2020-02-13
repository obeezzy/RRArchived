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

    explicit UpdateDebtor(const Utility::Debtor &debtor,
                          QObject *receiver);
    QueryResult execute() override;
private:
    void affirmDirtyTransactionsExist();
    void addNotesForNewTransactions(QVariantList &newDebtTransactionNoteIds);
    void updateNotesForExistingTransactions();
    void updateClientDetails();
    void addNewTransactions(const QVariantList &newDebtTransactionNoteIds,
                            QVariantList &newDebtTransactionIds);
    void updateExistingTransactions(QVariantList &updatedDebtTransactionIds);
    void archiveDeletedTransactions(QVariantList &archivedDebtTransactionIds);
    void addNewPayments(QVariantList &newDebtPaymentIds);
    void updateExistingPayments(QVariantList &updatedDebtPaymentIds);
    void archiveDeletedPayments(QVariantList &archivedDebtPaymentIds);
};
}

#endif // UPDATEDEBTOR_H
