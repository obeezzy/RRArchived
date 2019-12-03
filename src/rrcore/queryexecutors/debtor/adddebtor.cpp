#include "adddebtor.h"
#include "database/databaseutils.h"

using namespace DebtorQuery;

AddDebtor::AddDebtor(const QString &preferredName,
                     const QString &firstName,
                     const QString &lastName,
                     const QString &primaryPhoneNumber,
                     const QUrl &imageUrl,
                     const DebtTransactionList &newDebtTransactions,
                     const QString &note,
                     QObject *receiver) :
    DebtorExecutor(COMMAND, {
                        { "can_undo", true },
                        { "preferred_name", preferredName },
                        { "first_name", firstName },
                        { "last_name", lastName },
                        { "image", DatabaseUtils::imageToByteArray(imageUrl.toLocalFile()) },
                        { "primary_phone_number", primaryPhoneNumber },
                        { "new_debt_transactions", newDebtTransactions.toVariantList() },
                        { "note", note }
                   }, receiver)
{

}

QueryResult AddDebtor::execute()
{
    QueryResult result{ request() };

    return result;
}
