#include "updatesuspendedsaletransaction.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "database/databaseexception.h"
#include "database/exceptions/exceptions.h"
#include "user/userprofile.h"
#include "utility/sales/saletransaction.h"

using namespace Query::Sales;

UpdateSuspendedSaleTransaction::UpdateSuspendedSaleTransaction(
    const Utility::Sales::SaleTransaction& transaction, QObject* receiver)
    : SaleExecutor(
          COMMAND,
          {{"transaction_id", transaction.id},
           {"client_preferred_name", transaction.customer.client.preferredName},
           {"client_id", transaction.customer.client.id},
           {"customer_phone_number", transaction.customer.client.phoneNumber},
           {"total_cost", transaction.monies.totalCost.toDouble()},
           {"amount_paid", transaction.monies.amountPaid.toDouble()},
           {"balance", transaction.monies.balance.toDouble()},
           {"note_id", transaction.note.id},
           {"note", transaction.note.note},
           {"suspended",
            transaction.flags.testFlag(Utility::RecordGroup::Suspended)},
           {"currency", transaction.monies.amountPaid.currency().isoCode()}},
          receiver)
{}

QueryResult UpdateSuspendedSaleTransaction::execute()
{
    QueryResult result{request()};
    result.setSuccessful(true);

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QSqlQuery q(connection);

    try {
        QueryExecutor::beginTransaction(q);

        affirmTransactionHasBeenSuspended();
        SaleExecutor::addSaleTransaction(TransactionMode::SkipSqlTransaction);
        archiveSaleTransaction();

        QueryExecutor::commitTransaction(q);
        return result;
    } catch (const DatabaseException&) {
        QueryExecutor::rollbackTransaction(q);
        throw;
    }
}

void UpdateSuspendedSaleTransaction::affirmTransactionHasBeenSuspended()
{
    const QVariantMap& params{request().params()};

    const auto& records(callProcedure(
        "IsSaleTransactionSuspended",
        {ProcedureArgument{ProcedureArgument::Type::In, "sale_transaction_id",
                           params.value("sale_transaction_id")}}));

    if (records.isEmpty())
        throw UnexpectedResultException(QStringLiteral(
            "Expected a boolean for 'suspended', received nothing"));

    const bool suspended = records.first().value("suspended").toBool();
    if (!suspended)
        throw UnexpectedResultException(
            QStringLiteral("Transaction must be suspended."));
}

void UpdateSuspendedSaleTransaction::archiveSaleTransaction()
{
    const QVariantMap& params{request().params()};

    callProcedure(
        "ArchiveSaleTransaction",
        {ProcedureArgument{ProcedureArgument::Type::In, "sale_transaction_id",
                           params.value("sale_transaction_id")},
         ProcedureArgument{ProcedureArgument::Type::In, "user_id",
                           UserProfile::instance().userId()}});
}
