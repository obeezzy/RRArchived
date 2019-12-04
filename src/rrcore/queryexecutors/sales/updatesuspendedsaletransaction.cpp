#include "updatesuspendedsaletransaction.h"
#include "database/databaseexception.h"
#include "database/databaseutils.h"
#include "user/userprofile.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

using namespace SaleQuery;
UpdateSuspendedSaleTransaction::UpdateSuspendedSaleTransaction(qint64 transactionId,
                                                               const QString &customerName,
                                                               int clientId,
                                                               const QString &customerPhoneNumber,
                                                               qreal totalCost,
                                                               qreal amountPaid,
                                                               qreal balance,
                                                               const QString &note,
                                                               bool suspended,
                                                               QObject *receiver) :
    SaleExecutor(COMMAND, {
                    { "transaction_id", transactionId },
                    { "customer_name", customerName },
                    { "client_id", clientId },
                    { "customer_phone_number", customerPhoneNumber },
                    { "total_cost", totalCost },
                    { "amount_paid", amountPaid },
                    { "balance", balance },
                    { "note", note },
                    { "suspended", suspended },
                    { "user_id", UserProfile::instance().userId() }
                 }, receiver)
{

}

QueryResult UpdateSuspendedSaleTransaction::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request().params();

    QSqlQuery q(connection);

    try {
        DatabaseUtils::beginTransaction(q);

        const QList<QSqlRecord> &records(callProcedure("IsSaleTransactionSuspended", {
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

        result = SaleExecutor::addSaleTransaction(TransactionMode::SkipSqlTransaction);

        callProcedure("ArchiveSaleTransaction", {
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
        return result;
    } catch (DatabaseException &) {
        DatabaseUtils::rollbackTransaction(q);
        throw;
    }
}
