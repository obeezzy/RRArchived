#include "viewexpensetransactions.h"
#include "database/databaseexception.h"
#include "utility/commonutils.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDateTime>

using namespace ExpenseQuery;

ViewExpenseTransactions::ViewExpenseTransactions(const Utility::DateTimeSpan &dateTimeSpan,
                                                 const Utility::RecordGroup::Flags &flags,
                                                 QObject *receiver) :
    ExpenseExecutor(COMMAND, {
                    { "from", dateTimeSpan.from },
                    { "to", dateTimeSpan.to },
                    { "archived", flags.testFlag(Utility::RecordGroup::Archived) }
                    }, receiver)
{

}

QueryResult ViewExpenseTransactions::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request().params();

    QSqlQuery q(connection);

    try {
        QueryExecutor::enforceArguments({ "from",
                                          "to"
                                        }, params);

        const auto &records(callProcedure("ViewExpenseTransactions", {
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
