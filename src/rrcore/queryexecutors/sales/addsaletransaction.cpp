#include "addsaletransaction.h"
#include "database/databaseexception.h"
#include "database/databaseutils.h"
#include "user/userprofile.h"
#include "singletons/settings.h"
#include "utility/saleutils.h"
#include "database/exceptions/exceptions.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

using namespace SaleQuery;

AddSaleTransaction::AddSaleTransaction(const Utility::SaleTransaction &transaction,
                                       QObject *receiver) :
    SaleExecutor(COMMAND, {
                    { "can_undo", true },
                    { "sale_transaction_id", transaction.id },
                    { "client_preferred_name", transaction.customer.client.preferredName },
                    { "client_id", transaction.customer.client.id },
                    { "client_phone_number", transaction.customer.client.phoneNumber },
                    { "total_cost", transaction.totalCost },
                    { "amount_paid", transaction.amountPaid },
                    { "balance", transaction.balance },
                    { "suspended", transaction.flags.testFlag(Utility::RecordGroup::Suspended) },
                    { "due_date_time", transaction.dueDateTime },
                    { "action", transaction.action },
                    { "note", transaction.note.note },
                    { "payments", transaction.payments.toVariantList() },
                    { "products", transaction.products.toVariantList() }
                 }, receiver)
{

}

AddSaleTransaction::AddSaleTransaction(const QueryRequest &request,
                                       QObject *receiver) :
    SaleExecutor(COMMAND, request.params(), receiver)
{
}


QueryResult AddSaleTransaction::execute()
{
    if (canUndo() && isUndoSet())
        return undoAddSaleTransaction();

    return SaleExecutor::addSaleTransaction(TransactionMode::UseSqlTransaction);
}

QueryResult AddSaleTransaction::undoAddSaleTransaction()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    const QVariantMap &params = request().params();

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QSqlQuery q(connection);

    try {
        QueryExecutor::enforceArguments({ "sale_transaction_id" }, params);

        DatabaseUtils::beginTransaction(q);

        affirmSaleTransactionIdIsValid();
        archiveSaleTransaction();
        archiveDebtTransaction();
        archiveCreditTransaction();
        revertProductQuantityUpdate();

        DatabaseUtils::commitTransaction(q);
        return result;
    } catch (DatabaseException &) {
        DatabaseUtils::rollbackTransaction(q);
        throw;
    }
}

void AddSaleTransaction::affirmSaleTransactionIdIsValid()
{
    const QVariantMap &params = request().params();
    const int saleTransactionId = params.value("sale_transaction_id").toInt();

    if (saleTransactionId <= 0)
        throw InvalidArgumentException(QStringLiteral("Sale transaction ID is invalid."));
}

void AddSaleTransaction::archiveSaleTransaction()
{
    const QVariantMap &params = request().params();

    callProcedure("ArchiveSaleTransaction", {
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "archived",
                          false
                      },
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

void AddSaleTransaction::archiveDebtTransaction()
{
    const QVariantMap &params = request().params();

    callProcedure("ArchiveDebtTransactionByTransactionTable", {
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "archived",
                          false
                      },
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "transaction_table",
                          QStringLiteral("sale_transaction")
                      },
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "debt_transaction_id",
                          params.value("debt_transaction_id")
                      },
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "user_id",
                          UserProfile::instance().userId()
                      }
                  });
}

void AddSaleTransaction::archiveCreditTransaction()
{
    const QVariantMap &params = request().params();

    callProcedure("ArchiveCreditTransaction", {
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "archived",
                          false
                      },
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "transaction_table",
                          QStringLiteral("sale_transaction")
                      },
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "credit_transaction_id",
                          params.value("credit_transaction_id")
                      },
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "user_id",
                          UserProfile::instance().userId()
                      }
                  });
}

void AddSaleTransaction::revertProductQuantityUpdate()
{
    const QVariantMap &params = request().params();

    callProcedure("RevertSaleQuantityUpdate", {
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
