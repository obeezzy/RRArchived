#include "updatesuspendedpurchasetransaction.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "database/databaseexception.h"
#include "database/exceptions/exceptions.h"
#include "user/userprofile.h"
#include "utility/common/recordgroup.h"
#include "utility/purchase/purchasetransaction.h"

using namespace Query::Purchase;

UpdateSuspendedPurchaseTransaction::UpdateSuspendedPurchaseTransaction(
    const Utility::Purchase::PurchaseTransaction& transaction,
    QObject* receiver)
    : PurchaseExecutor(
          COMMAND,
          {{"transaction_id", transaction.id},
           {"client_id", transaction.vendor.id},
           {"vendor_name", transaction.vendor.client.preferredName},
           {"phone_number", transaction.vendor.client.phoneNumber},
           {"total_cost", transaction.monies.totalCost.toDouble()},
           {"amount_paid", transaction.monies.amountPaid.toDouble()},
           {"balance", transaction.monies.balance.toDouble()},
           {"suspended",
            transaction.flags.testFlag(Utility::RecordGroup::Suspended)},
           {"products", transaction.products.toVariantList()},
           {"note_id", transaction.note.id},
           {"note", transaction.note.note}},
          receiver)
{}

QueryResult UpdateSuspendedPurchaseTransaction::execute()
{
    QueryResult result{request()};
    result.setSuccessful(true);

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QSqlQuery q(connection);

    try {
        QueryExecutor::beginTransaction(q);

        affirmTransactionHasBeenSuspended();
        PurchaseExecutor::addPurchaseTransaction(
            TransactionMode::SkipSqlTransaction);
        archivePurchaseTransaction();

        QueryExecutor::commitTransaction(q);
        return result;
    } catch (const DatabaseException&) {
        QueryExecutor::rollbackTransaction(q);
        throw;
    }
}

void UpdateSuspendedPurchaseTransaction::affirmTransactionHasBeenSuspended()
{
    const QVariantMap& params{request().params()};

    const auto& records(callProcedure(
        "IsPurchaseTransactionSuspended",
        {ProcedureArgument{ProcedureArgument::Type::In, "transaction_id",
                           params.value("transaction_id")}}));

    if (records.isEmpty())
        throw UnexpectedResultException(QStringLiteral(
            "Expected a boolean for 'suspended', received nothing"));

    const bool suspended = records.first().value("suspended").toBool();
    if (!suspended)
        throw UnexpectedResultException(
            QStringLiteral("Transaction must be suspended."));
}

void UpdateSuspendedPurchaseTransaction::archivePurchaseTransaction()
{
    const QVariantMap& params{request().params()};

    callProcedure(
        "ArchivePurchaseTransaction",
        {ProcedureArgument{ProcedureArgument::Type::In, "transaction_id",
                           params.value("transaction_id")},
         ProcedureArgument{ProcedureArgument::Type::In, "user_id",
                           UserProfile::instance().userId()}});
}
