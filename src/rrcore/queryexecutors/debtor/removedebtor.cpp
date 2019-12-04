#include "removedebtor.h"
#include "database/databaseerror.h"
#include "database/databaseexception.h"
#include "user/userprofile.h"
#include "database/databaseutils.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

using namespace DebtorQuery;

RemoveDebtor::RemoveDebtor(int debtorId,
                           int debtorRow,
                           QObject *receiver) :
    DebtorExecutor(COMMAND, {
                        { "can_undo", true },
                        { "debtor_id", debtorId },
                        { "debtor_row", debtorRow }
                   }, receiver)
{

}

QueryResult RemoveDebtor::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request().params();

    QSqlQuery q(connection);

    try {
        QueryExecutor::enforceArguments({ "debtor_id" }, params);

        if (params.value("debtor_id").toInt() <= 0)
            throw DatabaseException(DatabaseError::QueryErrorCode::InvalidArguments, QString(),
                                    QStringLiteral("Debtor ID is invalid."));

        DatabaseUtils::beginTransaction(q);

        callProcedure("ArchiveDebtor", {
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "debtor_id",
                              params.value("debtor_id")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "user_id",
                              UserProfile::instance().userId()
                          }
                      });

        // Archive debt transactions.
        callProcedure("ArchiveDebtTransaction3", {
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "debtor_id",
                              params.value("debtor_id")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "user_id",
                              UserProfile::instance().userId()
                          }
                      });

        // Retrieve archived debt transaction IDs.
        const QList<QSqlRecord> records(callProcedure("GetDebtTransaction", {
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::In,
                                                              "debtor_id",
                                                              params.value("debtor_id")
                                                          }
                                                      }));

        QVariantList debtTransactionIds;
        for (const QSqlRecord &record : records) {
            debtTransactionIds.append(record.value("id").toInt());
        }

        result.setOutcome(QVariantMap{ { "debtor_id", params.value("debtor_id") },
                                       { "debt_transaction_ids", debtTransactionIds },
                                       { "record_count", QVariant(1) },
                                       { "debtor_row", params.value("debtor_row") }
                          });

        DatabaseUtils::commitTransaction(q);
        return result;
    } catch (DatabaseException &) {
        DatabaseUtils::rollbackTransaction(q);
        throw;
    }
}
