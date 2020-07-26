#include "filterpurchasereport.h"
#include "database/databaseexception.h"
#include "utility/common/filtercriteria.h"
#include "utility/common/sortcriteria.h"
#include "utility/common/datetimespan.h"

using namespace Query::Purchase;

FilterPurchaseReport::FilterPurchaseReport(const Utility::FilterCriteria &filterCriteria,
                                           const Utility::SortCriteria &sortCriteria,
                                           const Utility::DateTimeSpan &dateTimeSpan,
                                           QObject *receiver) :
    PurchaseExecutor(COMMAND, {
                        { "filter_column", filterCriteria.column },
                        { "filter_text", filterCriteria.text },
                        { "sort_column", sortCriteria.column },
                        { "sort_order", sortCriteria.orderAsString() },
                        { "from", dateTimeSpan.from },
                        { "to", dateTimeSpan.to }
                     }, receiver)
{}

QueryResult FilterPurchaseReport::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    const QVariantMap &params = request().params();

    try {
        const auto &records(callProcedure("FilterPurchaseReport", {
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

        QVariantList products;
        for (const auto &record : records)
            products.append(recordToMap(record));

        result.setOutcome(QVariantMap {
                              { "products", products },
                              { "record_count", products.count() },
                          });
        return result;
    } catch (const DatabaseException &) {
        throw;
    }
}
