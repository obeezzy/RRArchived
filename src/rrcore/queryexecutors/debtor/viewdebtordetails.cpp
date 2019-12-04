#include "viewdebtordetails.h"
#include "database/databaseexception.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

using namespace DebtorQuery;

ViewDebtorDetails::ViewDebtorDetails(int debtorId,
                                     QObject *receiver) :
    DebtorExecutor(COMMAND, {
                        { "debtor_id", debtorId }
                   }, receiver)
{

}

QueryResult ViewDebtorDetails::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request().params();
    QSqlQuery q(connection);

    try {
        QueryExecutor::enforceArguments({ "debtor_id" }, params);

        // STEP: Get total balance for each debtor
        const auto records = callProcedure("ViewDebtorDetails", {
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

        if (records.isEmpty())
            throw DatabaseException(DatabaseError::QueryErrorCode::InvalidArguments,
                                    QString(),
                                    QStringLiteral("Debtor with ID %1 does not exist.")
                                    .arg(params.value("debtor_id").toInt()));

        result.setOutcome(QVariantMap {
                              { "debtor", recordToMap(records.first()) },
                              { "record_count", 1 }
                          });
        return result;
    } catch (DatabaseException &) {
        throw;
    }
}
