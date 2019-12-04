#include "removepurchasetransaction.h"
#include "database/databaseexception.h"
#include "database/databaseutils.h"
#include "user/userprofile.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

using namespace PurchaseQuery;

RemovePurchaseTransaction::RemovePurchaseTransaction(qint64 transactionId,
                                                     int row,
                                                     const PurchaseTransaction &transaction,
                                                     QObject *receiver) :
    PurchaseExecutor(COMMAND, {
                            { "can_undo", true },
                            { "transaction_id", transactionId },
                            { "row", row },
                            { "record", transaction.toVariantMap() }
                     }, receiver)
{

}

QueryResult RemovePurchaseTransaction::execute()
{
    if (canUndo() && isUndoSet())
        return undoRemovePurchaseTransaction();

    return removePurchaseTransaction();
}

QueryResult RemovePurchaseTransaction::removePurchaseTransaction()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request().params();
    QSqlQuery q(connection);

    try {
        QueryExecutor::enforceArguments({ "transaction_id" }, params);

        DatabaseUtils::beginTransaction(q);

        callProcedure("ArchivePurchaseTransaction", {
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "archived",
                              true
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
                              true
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "transaction_table",
                              QStringLiteral("purchase_transaction")
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

        callProcedure("ArchiveCreditTransaction", {
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "archived",
                              true
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "transaction_table",
                              QStringLiteral("purchase_transaction")
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

        callProcedure("RevertPurchaseQuantityUpdate", {
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

        DatabaseUtils::commitTransaction(q);
        result.setOutcome(QVariantMap { { "transaction_id", params.value("transaction_id").toInt() }, { "record_count", 1 } });
        return result;
    } catch (DatabaseException &) {
        DatabaseUtils::rollbackTransaction(q);
        throw;
    }
}

QueryResult RemovePurchaseTransaction::undoRemovePurchaseTransaction()
{
    throw DatabaseException(DatabaseError::QueryErrorCode::NotYetImplementedError);
}
