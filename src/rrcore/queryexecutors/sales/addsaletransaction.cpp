#include "addsaletransaction.h"
#include "database/databaseexception.h"
#include "database/databaseutils.h"
#include "user/userprofile.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

using namespace SaleQuery;

AddSaleTransaction::AddSaleTransaction(qint64 transactionId,
                                       const QString &customerName,
                                       int clientId,
                                       const QString &customerPhoneNumber,
                                       qreal totalCost,
                                       qreal amountPaid,
                                       qreal balance,
                                       bool suspended,
                                       const QDateTime &dueDate,
                                       const QString &action,
                                       const QString &note,
                                       const SalePaymentList &payments,
                                       const StockItemList &items,
                                       QObject *receiver) :
    SaleExecutor(COMMAND, {
                    { "can_undo", true },
                    { "transaction_id", transactionId },
                    { "customer_name", customerName },
                    { "client_id", clientId },
                    { "customer_phone_number", customerPhoneNumber },
                    { "total_cost", totalCost },
                    { "amount_paid", amountPaid },
                    { "balance", balance },
                    { "suspended", suspended },
                    { "due_date", dueDate },
                    { "action", action },
                    { "note", note },
                    { "payments", payments.toVariantList() },
                    { "items", items.toVariantList() },
                    { "currency", "NGN" },
                    { "user_id", UserProfile::instance().userId() }
                 }, receiver)
{

}

AddSaleTransaction::AddSaleTransaction(const QueryRequest &request, QObject *receiver) :
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

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request().params();

    QSqlQuery q(connection);

    try {
        QueryExecutor::enforceArguments({ "transaction_id" }, params);

        DatabaseUtils::beginTransaction(q);

        if (params.value("transaction_id").toInt() > 0) {
            callProcedure("ArchiveSaleTransaction", {
                              ProcedureArgument {
                                  ProcedureArgument::Type::In,
                                  "archived",
                                  false
                              },
                              ProcedureArgument {
                                  ProcedureArgument::Type::In,
                                  "transaction_id",
                                  params.value("transaction_id")
                              },
                              ProcedureArgument {
                                  ProcedureArgument::Type::In,
                                  "user_id",
                                  params.value("user_id")
                              }
                          });

            callProcedure("ArchiveDebtTransaction1", {
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
                                  "transaction_id",
                                  params.value("transaction_id")
                              },
                              ProcedureArgument {
                                  ProcedureArgument::Type::In,
                                  "user_id",
                                  params.value("user_id")
                              }
                          });

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
                                  "transaction_id",
                                  params.value("transaction_id")
                              },
                              ProcedureArgument {
                                  ProcedureArgument::Type::In,
                                  "user_id",
                                  params.value("user_id")
                              }
                          });

            callProcedure("RevertSaleQuantityUpdate", {
                              ProcedureArgument {
                                  ProcedureArgument::Type::In,
                                  "transaction_id",
                                  params.value("transaction_id")
                              },
                              ProcedureArgument {
                                  ProcedureArgument::Type::In,
                                  "user_id",
                                  params.value("user_id")
                              }
                          });
        } else {
            throw DatabaseException(DatabaseError::QueryErrorCode::InvalidArguments,
                                    QStringLiteral("Transaction ID is not valid."),
                                    QStringLiteral("Transaction ID is not valid."));
        }

        DatabaseUtils::commitTransaction(q);
        return result;
    } catch (DatabaseException &) {
        DatabaseUtils::rollbackTransaction(q);
        throw;
    }
}
