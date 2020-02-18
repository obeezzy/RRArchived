#include "addpurchasetransaction.h"
#include "utility/purchaseutils.h"
#include "utility/commonutils.h"
#include "database/databaseexception.h"
#include "user/userprofile.h"
#include "database/exceptions/exceptions.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

using namespace PurchaseQuery;

AddPurchaseTransaction::AddPurchaseTransaction(const Utility::PurchaseTransaction &transaction,
                                               QObject *receiver) :
    PurchaseExecutor(COMMAND, {
                        { "can_undo", true },
                        { "purchase_transaction_id", transaction.id },
                        { "client_id", transaction.vendor.client.id },
                        { "vendor_name", transaction.vendor.client.preferredName },
                        { "phone_number", transaction.vendor.client.phoneNumber },
                        { "total_cost", transaction.totalCost },
                        { "amount_paid", transaction.amountPaid },
                        { "balance", transaction.balance },
                        { "suspended", transaction.flags.testFlag(Utility::RecordGroup::Suspended) },
                        { "due_date", transaction.dueDateTime },
                        { "action", transaction.action },
                        { "payments", transaction.payments.toVariantList() },
                        { "products", transaction.products.toVariantList() },
                        { "note", transaction.note.note }
                     }, receiver)
{

}

QueryResult AddPurchaseTransaction::execute()
{
    if (canUndo() && isUndoSet())
        return undoAddPurchaseTransaction();

    return PurchaseExecutor::addPurchaseTransaction(TransactionMode::UseSqlTransaction);
}

QueryResult AddPurchaseTransaction::undoAddPurchaseTransaction()
{
    QueryResult result{ request() };
    result.setSuccessful(false);

    const QVariantMap &params = request().params();
    const QString &transactionTable = QStringLiteral("purchase_transaction");
    const int transactionId = params.value("purchase_transaction_id").toInt();

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QSqlQuery q(connection);

    try {
        QueryExecutor::beginTransaction(q);

        if (transactionId > 0) {
            archivePurchaseTransaction(transactionId);
            archiveDebtTransaction(transactionTable,
                                   transactionId);
            archiveCreditTransaction(transactionTable,
                                     transactionId);
        }

        revertProductQuantityUpdate(transactionId);
        QueryExecutor::commitTransaction(q);

        result.setOutcome(params);
        return result;
    } catch (const DatabaseException &) {
        QueryExecutor::rollbackTransaction(q);
        throw;
    }
}

void AddPurchaseTransaction::archivePurchaseTransaction(int transactionId)
{
    if (transactionId <= 0)
        throw InvalidArgumentException(QStringLiteral("Invalid transaction ID."));

    callProcedure("ArchivePurchaseTransaction", {
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "archived",
                          false
                      },
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "purchase_transaction_id",
                          transactionId
                      },
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "user_id",
                          UserProfile::instance().userId()
                      }
                  });
}

void AddPurchaseTransaction::archiveDebtTransaction(const QString &transactionTable,
                                                    int transactionId)
{
    callProcedure("ArchiveDebtTransactionByTransactionTable", {
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "archived",
                          false
                      },
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "transaction_table",
                          transactionTable
                      },
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "transaction_id",
                          transactionId
                      },
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "user_id",
                          UserProfile::instance().userId()
                      }
                  });
}

void AddPurchaseTransaction::archiveCreditTransaction(const QString &transactionTable,
                                                      int transactionId)
{
    callProcedure("ArchiveCreditTransaction", {
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "transaction_table",
                          transactionTable
                      },
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "transaction_id",
                          transactionId
                      },
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "user_id",
                          UserProfile::instance().userId()
                      }
                  });
}

void AddPurchaseTransaction::revertProductQuantityUpdate(int transactionId)
{
    callProcedure("RevertPurchaseQuantityUpdate", {
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "purchase_transaction_id",
                          transactionId
                      },
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "user_id",
                          UserProfile::instance().userId()
                      }
                  });
}
