#include "updatedebtor.h"
#include "database/databaseutils.h"

using namespace DebtorQuery;

UpdateDebtor::UpdateDebtor(int debtorId,
                           int clientId,
                           const QString &preferredName,
                           const QString &firstName,
                           const QString &lastName,
                           const QString &primaryPhoneNumber,
                           const QUrl &imageUrl,
                           const DebtTransactionList &newDebtTransactions,
                           const DebtTransactionList &updatedDebtTransactions,
                           const QString &note,
                           QObject *receiver) :
    DebtorExecutor(COMMAND, {
                        { "can_undo", true },
                        { "debtor_id", debtorId },
                        { "client_id", clientId },
                        { "preferred_name", preferredName },
                        { "first_name", firstName },
                        { "last_name", lastName },
                        { "image", DatabaseUtils::imageToByteArray(imageUrl.toLocalFile()) },
                        { "primary_phone_number", primaryPhoneNumber },
                        { "new_debt_transactions", newDebtTransactions.toVariantList() },
                        { "updated_debt_transactions", updatedDebtTransactions.toVariantList() },
                        { "note", note }
                   }, receiver)
{

}

QueryResult UpdateDebtor::execute()
{
    QueryResult result{ request() };

    return result;
}
