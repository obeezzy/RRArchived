#include "removepurchasetransaction.h"
#include "database/databaseexception.h"
#include "database/databaseutils.h"
#include "user/userprofile.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

using namespace PurchaseQuery;

RemovePurchaseTransaction::RemovePurchaseTransaction(const PurchaseTransaction &transaction,
                                                     QObject *receiver) :
    PurchaseExecutor(COMMAND, {
                            { "can_undo", true },
                            { "purchase_transaction_id", transaction.id },
                            { "row", transaction.row },
                            { "transaction", transaction.toVariantMap() }
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

    const QVariantMap &params = request().params();

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QSqlQuery q(connection);

    try {
        QueryExecutor::enforceArguments({ "transaction_id" }, params);

        DatabaseUtils::beginTransaction(q);

        archivePurchaseTransaction();
        archiveDebtTransaction();
        archiveCreditTransaction();
        revertProductQuantityUpdate();

        DatabaseUtils::commitTransaction(q);
        result.setOutcome(QVariantMap {
                              { "transaction_id", params.value("transaction_id").toInt() },
                              { "record_count", 1 }
                          });
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

void RemovePurchaseTransaction::archivePurchaseTransaction()
{
    const QVariantMap &params = request().params();

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
}

void RemovePurchaseTransaction::archiveDebtTransaction()
{
    const QVariantMap &params = request().params();

    callProcedure("ArchiveDebtTransactionByTransactionTable", {
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
}

void RemovePurchaseTransaction::archiveCreditTransaction()
{
    const QVariantMap &params = request().params();

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
}

void RemovePurchaseTransaction::revertProductQuantityUpdate()
{
    const QVariantMap &params = request().params();

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
}
