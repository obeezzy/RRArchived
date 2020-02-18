#include "viewexpensereport.h"
#include "database/databaseexception.h"
#include "utility/commonutils.h"

using namespace ExpenseQuery;

ViewExpenseReport::ViewExpenseReport(const Utility::DateTimeSpan &dateTimeSpan,
                                     QObject *receiver) :
    ExpenseExecutor(COMMAND, {
                        { "from", dateTimeSpan.from },
                        { "to", dateTimeSpan.to }
                    }, receiver)
{

}

QueryResult ViewExpenseReport::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    const QVariantMap &params = request().params();

    try {
        const auto &records(callProcedure("ViewExpenseReport", {
                                              ProcedureArgument {
                                                  ProcedureArgument::Type::In,
                                                  "from",
                                                  params.value("from")
                                              },
                                              ProcedureArgument {
                                                  ProcedureArgument::Type::In,
                                                  "to",
                                                  params.value("to")
                                              }
                                          }));

        QVariantList transactions;
        for (const auto &record : records)
            transactions.append(recordToMap(record));

        result.setOutcome(QVariantMap {
                              { "transactions", transactions },
                              { "record_count", transactions.count() },
                          });
        return result;
    } catch (const DatabaseException &) {
        throw;
    }
}
