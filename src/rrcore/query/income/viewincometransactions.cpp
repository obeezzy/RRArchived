#include "viewincometransactions.h"
#include "database/databaseexception.h"
#include "utility/common/datetimespan.h"
#include "utility/common/recordgroup.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>

using namespace Query::Income;

ViewIncomeTransactions::ViewIncomeTransactions(const Utility::DateTimeSpan &dateTimeSpan,
                                               const Utility::RecordGroup::Flags &flags,
                                               QObject *receiver) :
    IncomeExecutor(COMMAND, {
                    { "from", dateTimeSpan.from },
                    { "to", dateTimeSpan.to },
                    { "archived", flags.testFlag(Utility::RecordGroup::Archived) }
                   }, receiver)
{

}

QueryResult ViewIncomeTransactions::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    const QVariantMap &params = request().params();

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QSqlQuery q(connection);

    try {
        const auto &records (callProcedure("ViewIncomeTransactions", {
                                               ProcedureArgument {
                                                   ProcedureArgument::Type::In,
                                                   "from",
                                                   params.value("from")
                                               },
                                               ProcedureArgument {
                                                   ProcedureArgument::Type::In,
                                                   "to",
                                                   params.value("to")
                                               },
                                               ProcedureArgument {
                                                   ProcedureArgument::Type::In,
                                                   "archived",
                                                   params.value("archived")
                                               }
                                           }));

        QVariantList transactions;
        for (const auto &record : records)
            transactions.append(recordToMap(record));

        result.setOutcome(QVariantMap {
                              { "transactions", transactions },
                              { "record_count", transactions.count() }
                          });

        return result;
    } catch (const DatabaseException &) {
        throw;
    }
}
