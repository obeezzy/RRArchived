#include "viewdebtpayments.h"
#include "database/databaseexception.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>

using namespace DebtorQuery;

ViewDebtPayments::ViewDebtPayments(int debtTransactionId,
                                   QObject *receiver) :
    DebtorExecutor(COMMAND, { { "debt_transaction_id", debtTransactionId } }, receiver)
{

}

QueryResult ViewDebtPayments::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    const QVariantMap &params{request().params()};
    QVariantList payments;

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QSqlQuery q(connection);

    try {
        QueryExecutor::enforceArguments({ "debt_transaction_id" }, params);
        fetchDebtPayments(payments);
        result.setOutcome(QVariantMap {
                              { "payments", payments },
                              { "record_count", payments.count() }
                          });
        return result;
    } catch (const DatabaseException &) {
        throw;
    }
}

void ViewDebtPayments::fetchDebtPayments(QVariantList &debtPayments)
{
    const QVariantMap &params{request().params()};

    const auto &records = callProcedure("ViewDebtPayments", {
                                            ProcedureArgument {
                                                ProcedureArgument::Type::In,
                                                "debt_transaction_id",
                                                params.value("debt_transaction_id")
                                            },
                                            ProcedureArgument {
                                                ProcedureArgument::Type::In,
                                                "archived",
                                                params.value("archived", false)
                                            }
                                        });

    for (const auto &record : records)
        debtPayments.append(recordToMap(record));
}
