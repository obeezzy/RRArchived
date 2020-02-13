#include "removedebtor.h"
#include "database/databaseerror.h"
#include "database/databaseexception.h"
#include "user/userprofile.h"
#include "database/databaseutils.h"
#include "database/exceptions/exceptions.h"
#include "utility/debtorutils.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

using namespace DebtorQuery;

RemoveDebtor::RemoveDebtor(const Utility::Debtor &debtor,
                           QObject *receiver) :
    DebtorExecutor(COMMAND, {
                    { "can_undo", true },
                    { "debtor_id", debtor.id },
                    { "row", debtor.row }
                   }, receiver)
{

}

QueryResult RemoveDebtor::execute()
{
    if (canUndo() && isUndoSet())
        return undoRemoveDebtor();

    return removeDebtor();
}

QueryResult RemoveDebtor::removeDebtor()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    const QVariantMap &params = request().params();
    const int debtorId = params.value("debtor_id").toInt();
    QVariantList archivedDebtTransactionIds;

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QSqlQuery q(connection);

    try {
        QueryExecutor::enforceArguments({ "debtor_id" }, params);

        if (debtorId <= 0)
            throw InvalidArgumentException(QStringLiteral("Debtor ID is invalid."));

        DatabaseUtils::beginTransaction(q);

        archiveDebtor();
        archiveDebtTransactions();
        fetchArchivedDebtTransactionIds(archivedDebtTransactionIds);

        result.setOutcome(QVariantMap {
                              { "debtor_id", params.value("debtor_id") },
                              { "debt_transaction_ids", archivedDebtTransactionIds },
                              { "record_count", QVariant(1) },
                              { "row", params.value("row") }
                          });

        DatabaseUtils::commitTransaction(q);
        return result;
    } catch (DatabaseException &) {
        DatabaseUtils::rollbackTransaction(q);
        throw;
    }
}

QueryResult RemoveDebtor::undoRemoveDebtor()
{
    QueryResult result{ request() };
    result.setSuccessful(false);

    const QVariantMap &params = request().params();
    const QVariantList &debtTransactionIds = params.value("outcome").toMap().value("debt_transaction_ids").toList();
    const int debtorId = params.value("debtor_id").toInt();
    QVariantMap debtor;

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QSqlQuery q(connection);

    try {
        QueryExecutor::enforceArguments({ "debtor_id" }, params);

        if (debtorId <= 0)
            throw InvalidArgumentException("Debtor ID is invalid.");

        DatabaseUtils::beginTransaction(q);

        unarchiveDebtor();
        unarchiveDebtTransactions(debtTransactionIds);
        fetchDebtor(debtor);

        result.setOutcome(QVariantMap {
                              { "debtor", QVariant{ debtor } },
                              { "record_count", 1 },
                              { "debtor_id", params.value("debtor_id") },
                              { "row", params.value("row") }
                          });

        DatabaseUtils::commitTransaction(q);
        return result;
    } catch (DatabaseException &) {
        DatabaseUtils::rollbackTransaction(q);
        throw;
    }
}

void RemoveDebtor::archiveDebtor()
{
    const QVariantMap &params = request().params();
    const int debtorId = params.value("debtor_id").toInt();

    callProcedure("ArchiveDebtor", {
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "debtor_id",
                          debtorId
                      },
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "user_id",
                          UserProfile::instance().userId()
                      }
                  });
}

void RemoveDebtor::archiveDebtTransactions()
{
    const QVariantMap &params = request().params();
    const int debtorId = params.value("debtor_id").toInt();

    callProcedure("ArchiveDebtTransactionByDebtorId", {
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "debtor_id",
                          debtorId
                      },
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "user_id",
                          UserProfile::instance().userId()
                      }
                  });
}

void RemoveDebtor::fetchArchivedDebtTransactionIds(QVariantList &archivedDebtTransactionIds)
{
    const QVariantMap &params = request().params();
    const int debtorId = params.value("debtor_id").toInt();

    const auto &records(callProcedure("FetchDebtTransaction", {
                                          ProcedureArgument {
                                              ProcedureArgument::Type::In,
                                              "debtor_id",
                                              debtorId
                                          }
                                      }));

    for (const auto &record : records)
        archivedDebtTransactionIds.append(record.value("debt_transaction_id"));
}

void RemoveDebtor::unarchiveDebtor()
{
    const QVariantMap &params = request().params();
    const int debtorId = params.value("debtor_id").toInt();

    callProcedure("UndoArchiveDebtor", {
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "debtor_id",
                          debtorId
                      },
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "user_id",
                          UserProfile::instance().userId()
                      }
                  });
}

void RemoveDebtor::unarchiveDebtTransactions(const QVariantList &debtTransactionIds)
{
    for (const auto &transactionId : debtTransactionIds) {
        callProcedure("UndoArchiveDebtTransaction", {
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "debt_transaction_id",
                              transactionId
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "user_id",
                              UserProfile::instance().userId()
                          }
                      });
    }
}

void RemoveDebtor::fetchDebtor(QVariantMap &debtor)
{
    const QVariantMap &params = request().params();
    const int debtorId = params.value("debtor_id").toInt();

    const auto &records = callProcedure("FetchDebtor", {
                                            ProcedureArgument {
                                                ProcedureArgument::Type::In,
                                                "debtor_id",
                                                debtorId
                                            }
                                        });

    if (records.isEmpty())
        throw NoExistingRecordException(QStringLiteral("Debtor with ID '%1' does not exist.")
                                        .arg(debtorId));

    debtor = recordToMap(records.first());
}
