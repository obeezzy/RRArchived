#include "addpurchasetransaction.h"
#include "database/databaseexception.h"
#include "database/databaseutils.h"
#include "user/userprofile.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

using namespace PurchaseQuery;

AddPurchaseTransaction::AddPurchaseTransaction(qint64 transactionId,
                                               int clientId,
                                               const QString &customerName, const QString &customerPhoneNumber,
                                               qreal totalCost,
                                               qreal amountPaid,
                                               qreal balance,
                                               bool suspended,
                                               const QDateTime &dueDate,
                                               const QString &action,
                                               const PurchasePaymentList &payments,
                                               const StockItemList &items,
                                               const QString &note,
                                               QObject *receiver) :
    PurchaseExecutor(COMMAND, {
                        { "can_undo", true },
                        { "transaction_id", transactionId },
                        { "client_id", clientId },
                        { "customer_name", customerName },
                        { "customer_phone_number", customerPhoneNumber },
                        { "total_cost", totalCost },
                        { "amount_paid", amountPaid },
                        { "balance", balance },
                        { "suspended", suspended },
                        { "due_date", dueDate },
                        { "action", action },
                        { "payments", payments.toVariantList() },
                        { "items", items.toVariantList() },
                        { "note", note },
                        { "transaction_table", QStringLiteral("purchase_transaction") },
                        { "user_id", UserProfile::instance().userId() }
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
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request().params();

    QSqlQuery q(connection);

    try {
        DatabaseUtils::beginTransaction(q);

        if (params.value("transaction_id").toInt() > 0) {
            callProcedure("ArchivePurchaseTransaction", {
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
                                  UserProfile::instance().userId()
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
                                  params.value("transaction_table")
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
                                  "transaction_table",
                                  params.value("transaction_table")
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
        }

        callProcedure("RevertPurchaseQuantityUpdate", {
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

        DatabaseUtils::commitTransaction(q);
        result.setOutcome(params);
        return result;
    } catch (DatabaseException &) {
        DatabaseUtils::rollbackTransaction(q);
        throw;
    }
}
