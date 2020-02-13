#ifndef ADDDEBTOR_H
#define ADDDEBTOR_H

#include "debtorexecutor.h"
#include <QUrl>

namespace Utility {
class Debtor;
}

namespace DebtorQuery {
class AddDebtor : public DebtorExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("add_debtor");
    static inline const QString UNDO_COMMAND = QStringLiteral("undo_add_debtor");

    explicit AddDebtor(const Utility::Debtor &debtor,
                       QObject *receiver);
    QueryResult execute() override;
private:
    QueryResult addDebtor();
    QueryResult undoAddDebtor();

    void affirmDebtorDoesNotExist(const QString &phoneNumber);
    int addNotesForDebtor(const QVariantMap &params,
                          const QVariantList &newDebtTransactions,
                          QVariantList &debtTransactionNoteIds);
    int addClient(const QVariantMap &params,
                  int noteId);
    int addDebtorToDatabase(int clientId,
                            int noteId);
    void addDebtTransactions(int debtorId,
                             const QVariantList &newDebtTransactions,
                             const QVariantList &debtTransactionNoteIds,
                             QVariantList &newDebtTransactionIds,
                             QVariantList &newDebtPaymentIds);
    void addDebtPayments(int debtTransactionId,
                         const QVariantList &newDebtPayments,
                         QVariantList &newDebtPaymentIds);
};
}

#endif // ADDDEBTOR_H
