#include "updatesuspendedsaletransaction.h"
#include "database/databaseexception.h"
#include "database/databaseutils.h"
#include "user/userprofile.h"
#include "utility/saleutils.h"
#include "database/exceptions/exceptions.h"
#include "singletons/settings.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

using namespace SaleQuery;

UpdateSuspendedSaleTransaction::UpdateSuspendedSaleTransaction(const Utility::SaleTransaction &transaction,
                                                               QObject *receiver) :
    SaleExecutor(COMMAND, {
                    { "transaction_id", transaction.id },
                    { "client_preferred_name", transaction.customer.client.preferredName },
                    { "client_id", transaction.customer.client.id },
                    { "customer_phone_number", transaction.customer.client.phoneNumber },
                    { "total_cost", transaction.totalCost },
                    { "amount_paid", transaction.amountPaid },
                    { "balance", transaction.balance },
                    { "note_id", transaction.note.id },
                    { "note", transaction.note.note },
                    { "suspended", transaction.flags.testFlag(Utility::RecordGroup::Suspended) },
                    { "currency", Settings::DEFAULT_CURRENCY }
                 }, receiver)
{

}

QueryResult UpdateSuspendedSaleTransaction::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QSqlQuery q(connection);

    try {
        DatabaseUtils::beginTransaction(q);

        affirmTransactionHasBeenSuspended();
        SaleExecutor::addSaleTransaction(TransactionMode::SkipSqlTransaction);
        archiveSaleTransaction();

        DatabaseUtils::commitTransaction(q);
        return result;
    } catch (const DatabaseException &) {
        DatabaseUtils::rollbackTransaction(q);
        throw;
    }
}

void UpdateSuspendedSaleTransaction::affirmTransactionHasBeenSuspended()
{
    const QVariantMap &params = request().params();

    const auto &records(callProcedure("IsSaleTransactionSuspended", {
                                          ProcedureArgument {
                                              ProcedureArgument::Type::In,
                                              "sale_transaction_id",
                                              params.value("sale_transaction_id")
                                          }
                                      }));

    if (records.isEmpty())
        throw UnexpectedResultException(QStringLiteral("Expected a boolean for 'suspended', received nothing"));

    const bool suspended = records.first().value("suspended").toBool();
    if (!suspended)
        throw UnexpectedResultException(QStringLiteral("Transaction must be suspended."));
}

void UpdateSuspendedSaleTransaction::archiveSaleTransaction()
{
    const QVariantMap &params = request().params();

    callProcedure("ArchiveSaleTransaction", {
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "sale_transaction_id",
                          params.value("sale_transaction_id")
                      },
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "user_id",
                          UserProfile::instance().userId()
                      }
                  });
}
