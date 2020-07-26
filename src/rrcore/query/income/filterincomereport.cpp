#include "filterincomereport.h"
#include "utility/common/filtercriteria.h"
#include "utility/common/sortcriteria.h"
#include "utility/common/datetimespan.h"
#include "database/databaseexception.h"

using namespace Query::Income;

FilterIncomeReport::FilterIncomeReport(const Utility::FilterCriteria &filterCriteria,
                                       const Utility::SortCriteria &sortCriteria,
                                       const Utility::DateTimeSpan &dateTimeSpan,
                                       QObject *receiver) :
    IncomeExecutor(COMMAND, {
                    { "from", dateTimeSpan.from },
                    { "to", dateTimeSpan.to },
                    { "filter_text", filterCriteria.text },
                    { "filter_column", filterCriteria.column },
                    { "sort_column", sortCriteria.column },
                    { "sort_order", sortCriteria.orderAsString() }
                   }, receiver)
{

}

QueryResult FilterIncomeReport::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    const QVariantMap &params = request().params();

    try {
        const auto &records(callProcedure("FilterIncomeReport", {
                                              ProcedureArgument {
                                                  ProcedureArgument::Type::In,
                                                  "filter_column",
                                                  params.value("filter_column")
                                              },
                                              ProcedureArgument {
                                                  ProcedureArgument::Type::In,
                                                  "filter_text",
                                                  params.value("filter_text")
                                              },
                                              ProcedureArgument {
                                                  ProcedureArgument::Type::In,
                                                  "sort_column",
                                                  params.value("sort_column")
                                              },
                                              ProcedureArgument {
                                                  ProcedureArgument::Type::In,
                                                  "sort_order",
                                                  params.value("sort_order")
                                              },
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
