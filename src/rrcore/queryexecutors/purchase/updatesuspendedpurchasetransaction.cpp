#include "updatesuspendedpurchasetransaction.h"
#include "database/databaseutils.h"
#include "database/databaseexception.h"
#include "utility/purchaseutils.h"
#include "user/userprofile.h"
#include "database/exceptions/exceptions.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

using namespace PurchaseQuery;

UpdateSuspendedPurchaseTransaction::UpdateSuspendedPurchaseTransaction(const Utility::PurchaseTransaction &transaction,
                                                                       QObject *receiver) :
    PurchaseExecutor(COMMAND, {
                            { "transaction_id", transaction.id },
                            { "client_id", transaction.vendor.id },
                            { "vendor_name", transaction.vendor.client.preferredName },
                            { "phone_number", transaction.vendor.client.phoneNumber },
                            { "total_cost", transaction.totalCost },
                            { "amount_paid", transaction.amountPaid },
                            { "balance", transaction.balance },
                            { "suspended", transaction.flags.testFlag(Utility::RecordGroup::Suspended) },
                            { "products", transaction.products.toVariantList() },
                            { "note_id", transaction.note.id },
                            { "note", transaction.note.note }
                     }, receiver)
{

}

QueryResult UpdateSuspendedPurchaseTransaction::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QSqlQuery q(connection);

    try {
        DatabaseUtils::beginTransaction(q);

        affirmTransactionHasBeenSuspended();
        PurchaseExecutor::addPurchaseTransaction(TransactionMode::SkipSqlTransaction);
        archivePurchaseTransaction();

        DatabaseUtils::commitTransaction(q);
        return result;
    } catch (const DatabaseException &) {
        DatabaseUtils::rollbackTransaction(q);
        throw;
    }
}

void UpdateSuspendedPurchaseTransaction::affirmTransactionHasBeenSuspended()
{
    const QVariantMap &params = request().params();

    const auto &records(callProcedure("IsPurchaseTransactionSuspended", {
                                                       ProcedureArgument {
                                                           ProcedureArgument::Type::In,
                                                           "transaction_id",
                                                           params.value("transaction_id")
                                                       }
                                                   }));

    if (records.isEmpty())
        throw UnexpectedResultException(QStringLiteral("Expected a boolean for 'suspended', received nothing"));

    const bool suspended = records.first().value("suspended").toBool();
    if (!suspended)
        throw UnexpectedResultException(QStringLiteral("Transaction must be suspended."));
}

void UpdateSuspendedPurchaseTransaction::archivePurchaseTransaction()
{
    const QVariantMap &params = request().params();

    callProcedure("ArchivePurchaseTransaction", {
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "transaction_id",
                          params.value("transaction_id")
                      },
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "user_id",
                          UserProfile::instance().userId()
                      }
                  });
}
