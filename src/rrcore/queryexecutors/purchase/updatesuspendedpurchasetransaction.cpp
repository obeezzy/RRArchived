#include "updatesuspendedpurchasetransaction.h"
#include "database/databaseutils.h"
#include "database/databaseexception.h"
#include "user/userprofile.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

using namespace PurchaseQuery;

UpdateSuspendedPurchaseTransaction::UpdateSuspendedPurchaseTransaction(qint64 transactionId,
                                                                       int clientId,
                                                                       const QString &customerName,
                                                                       const QString &customerPhoneNumber,
                                                                       qreal totalCost,
                                                                       qreal amountPaid,
                                                                       qreal balance,
                                                                       bool suspended,
                                                                       const StockItemList &items,
                                                                       const QString &note,
                                                                       QObject *receiver) :
    PurchaseExecutor(COMMAND, {
                            { "transaction_id", transactionId },
                            { "client_id", clientId },
                            { "customer_name", customerName },
                            { "customer_phone_number", customerPhoneNumber },
                            { "total_cost", totalCost },
                            { "amount_paid", amountPaid },
                            { "balance", balance },
                            { "suspended", suspended },
                            { "items", items.toVariantList() },
                            { "note", note }
                     }, receiver)
{

}

QueryResult UpdateSuspendedPurchaseTransaction::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request().params();

    QSqlQuery q(connection);

    try {
        DatabaseUtils::beginTransaction(q);

        const QList<QSqlRecord> &records(callProcedure("IsPurchaseTransactionSuspended", {
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "transaction_id",
                                                               params.value("transaction_id")
                                                           }
                                                       }));

        if (!records.first().value("suspended").toBool())
            throw DatabaseException(DatabaseError::QueryErrorCode::UpdateTransactionFailure,
                                    QString(),
                                    "Transaction must be suspended.");

        PurchaseExecutor::addPurchaseTransaction(TransactionMode::SkipSqlTransaction);

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

        DatabaseUtils::commitTransaction(q);
        return result;
    } catch (DatabaseException &) {
        DatabaseUtils::rollbackTransaction(q);
        throw;
    }
}
