#include "viewincomereport.h"
#include "database/databaseexception.h"
#include "query/income/viewincomereport.h"
#include "utility/common/datetimespan.h"

using namespace Query::Income;

ViewIncomeReport::ViewIncomeReport(const Utility::DateTimeSpan& dateTimeSpan,
                                   QObject* receiver)
    : IncomeExecutor(COMMAND,
                     {{"from", dateTimeSpan.from}, {"to", dateTimeSpan.to}},
                     receiver)
{}

QueryResult Query::Income::ViewIncomeReport::execute()
{
    QueryResult result{request()};
    result.setSuccessful(true);

    const QVariantMap& params = request().params();

    try {
        const auto& records(
            callProcedure("ViewIncomeReport",
                          {ProcedureArgument{ProcedureArgument::Type::In,
                                             "from", params.value("from")},
                           ProcedureArgument{ProcedureArgument::Type::In, "to",
                                             params.value("to")}}));

        QVariantList transactions;
        for (const auto& record : records)
            transactions.append(recordToMap(record));

        result.setOutcome(QVariantMap{
            {"transactions", transactions},
            {"record_count", transactions.count()},
        });
        return result;
    } catch (const DatabaseException&) {
        throw;
    }
}
