#include "viewdebttransactions.h"
#include "database/databaseexception.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>

using namespace DebtorQuery;

ViewDebtTransactions::ViewDebtTransactions(int debtorId,
                                           QObject *receiver) :
    DebtorExecutor(COMMAND, { { "debtor_id", debtorId } }, receiver)
{

}

QueryResult ViewDebtTransactions::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    const QVariantMap &params = request().params();
    QVariantMap debtorDetails;
    QVariantList transactions;

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QSqlQuery q(connection);

    try {
        QueryExecutor::enforceArguments({ "debtor_id" }, params);

        fetchDebtorDetails(params, debtorDetails);
        fetchDebtTransactions(params, transactions);

        qDebug() << "Transactions?????" << transactions;

        result.setOutcome(QVariantMap {
                              { "client_id", debtorDetails.value("client_id") },
                              { "preferred_name", debtorDetails.value("preferred_name") },
                              { "primary_phone_number", debtorDetails.value("primary_phone_number") },
                              { "note", debtorDetails.value("note") },
                              { "debtor_id", params.value("debtor_id") },
                              { "transactions", transactions },
                              { "record_count", transactions.count() }
                          });
        return result;
    } catch (DatabaseException &) {
        throw;
    }
}

void ViewDebtTransactions::fetchDebtorDetails(const QVariantMap &params,
                                              QVariantMap &debtorDetails)
{
    const QList<QSqlRecord> records = callProcedure("ViewFewDebtorDetails", {
                                                        ProcedureArgument {
                                                            ProcedureArgument::Type::In,
                                                            "debtor_id",
                                                            params.value("debtor_id")
                                                        },
                                                        ProcedureArgument {
                                                            ProcedureArgument::Type::In,
                                                            "archived",
                                                            params.value("archived", false)
                                                        }
                                                    });

    if (!records.isEmpty()) {
        debtorDetails.insert("client_id", records.first().value("client_id").toInt());
        debtorDetails.insert("preferred_name", records.first().value("preferred_name").toInt());
        debtorDetails.insert("primary_phone_number", records.first().value("primary_phone_number").toInt());
        debtorDetails.insert("note", records.first().value("note").toInt());
    }
}

void ViewDebtTransactions::fetchDebtTransactions(const QVariantMap &params, QVariantList &debtTransactions)
{
    QList<QSqlRecord> records = callProcedure("ViewDebtTransactions", {
                                                  ProcedureArgument {
                                                      ProcedureArgument::Type::In,
                                                      "debtor_id",
                                                      params.value("debtor_id")
                                                  },
                                                  ProcedureArgument {
                                                      ProcedureArgument::Type::In,
                                                      "archived",
                                                      params.value("archived", false)
                                                  }
                                              });

    for (const QSqlRecord &record : records)
        debtTransactions.append(recordToMap(record));
}
