#include "fetchdebtor.h"
#include "database/databaseexception.h"
#include "database/exceptions/exceptions.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

using namespace DebtorQuery;

FetchDebtor::FetchDebtor(int debtorId,
                         QObject *receiver) :
    DebtorExecutor(COMMAND, {
                    { "debtor_id", debtorId }
                   }, receiver)
{

}

QueryResult FetchDebtor::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request().params();
    QSqlQuery q(connection);

    try {
        QueryExecutor::enforceArguments({ "debtor_id" }, params);

        const auto records = callProcedure("FetchDebtor", {
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
            throw NoExistingRecordException(QStringLiteral("Debtor with ID %1 does not exist.")
                                            .arg(params.value("debtor_id").toInt()));

        QVariantMap debtor(recordToMap(records.first()));
        debtor.insert("record_count", 1);
        result.setOutcome(debtor);
        return result;
    } catch (const DatabaseException &) {
        throw;
    }
}
