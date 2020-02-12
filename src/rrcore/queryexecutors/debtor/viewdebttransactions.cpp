#include "viewdebttransactions.h"
#include "database/databaseexception.h"
#include "utility/debtorutils.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>

using namespace DebtorQuery;

ViewDebtTransactions::ViewDebtTransactions(const Debtor &debtor,
                                           QObject *receiver) :
    DebtorExecutor(COMMAND, { { "debtor_id", debtor.id } }, receiver)
{

}

QueryResult ViewDebtTransactions::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    const QVariantMap &params = request().params();
    QVariantMap debtor;
    QVariantList transactions;

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QSqlQuery q(connection);

    try {
        QueryExecutor::enforceArguments({ "debtor_id" }, params);

        fetchDebtor(debtor);
        fetchDebtTransactions(transactions);

        result.setOutcome(QVariantMap {
                              { "client_id", debtor.value("client_id") },
                              { "preferred_name", debtor.value("preferred_name") },
                              { "phone_number", debtor.value("phone_number") },
                              { "note", debtor.value("note") },
                              { "debtor_id", params.value("debtor_id") },
                              { "transactions", transactions },
                              { "record_count", transactions.count() }
                          });
        return result;
    } catch (DatabaseException &) {
        throw;
    }
}

void ViewDebtTransactions::fetchDebtor(QVariantMap &debtor)
{
    const QVariantMap &params = request().params();

    const auto &records = callProcedure("FetchDebtor", {
                                            ProcedureArgument {
                                                ProcedureArgument::Type::In,
                                                "debtor_id",
                                                params.value("debtor_id")
                                            },
                                            ProcedureArgument {
                                                ProcedureArgument::Type::In,
                                                "archived",
                                                params.value("archived")
                                            }
                                        });

    if (!records.isEmpty()) {
        debtor.insert("client_id", records.first().value("client_id").toInt());
        debtor.insert("preferred_name", records.first().value("preferred_name").toInt());
        debtor.insert("phone_number", records.first().value("phone_number").toInt());
        debtor.insert("note", records.first().value("note").toInt());
    }
}

void ViewDebtTransactions::fetchDebtTransactions(QVariantList &debtTransactions)
{
    const QVariantMap &params = request().params();
    const auto &records = callProcedure("ViewDebtTransactions", {
                                            ProcedureArgument {
                                                ProcedureArgument::Type::In,
                                                "debtor_id",
                                                params.value("debtor_id")
                                            },
                                            ProcedureArgument {
                                                ProcedureArgument::Type::In,
                                                "archived",
                                                params.value("archived")
                                            }
                                        });

    for (const auto &record : records)
        debtTransactions.append(recordToMap(record));
}
