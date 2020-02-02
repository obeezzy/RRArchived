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

    explicit UpdateDebtor(const DebtorDetails &debtorDetails,
                          const DebtTransactionList &debtTransactions,
                          QObject *receiver);
    QueryResult execute() override;
private:
    void addNotesForNewTransactions(const QVariantList &newDebtTransactions,
                                    QVariantList &newDebtTransactionNoteIds);
    void updateNotesForExistingTransactions(const QVariantList &updatedDebtTransactions);
    void updateClientDetails(int clientId, const QVariantMap &params);
    void addNewTransactions(int debtorId,
                            const QVariantList &newDebtTransactions,
                            const QVariantList &newDebtTransactionNoteIds,
                            QVariantList &newDebtTransactionIds);
    void updateExistingTransactions(const QVariantList &updatedDebtTransactions,
                                    QVariantList &updatedDebtTransactionIds);
    void archiveDeletedTransactions(const QVariantList &archivedDebtTransactions,
                                    QVariantList &archivedDebtTransactionIds);
    void addNewPayments(const QVariantList &newDebtPayments,
                        QVariantList &newDebtPaymentIds);
    void updateExistingPayments(const QVariantList &updatedDebtPayments,
                                QVariantList &updatedDebtPaymentIds);
    void archiveDeletedPayments(const QVariantList &archivedDebtPayments,
                                QVariantList &archivedDebtPaymentIds);
};
}

#endif // UPDATEDEBTOR_H
