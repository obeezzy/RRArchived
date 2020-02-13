#include "viewsalereport.h"
#include "database/databaseexception.h"
#include "utility/commonutils.h"

using namespace SaleQuery;

ViewSaleReport::ViewSaleReport(const Utility::DateTimeSpan &dateTimeSpan,
                               QObject *receiver) :
    SaleExecutor(COMMAND, {
                    { "from", dateTimeSpan.from },
                    { "to", dateTimeSpan.to }
                 }, receiver)
{

}

QueryResult ViewSaleReport::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    const QVariantMap &params = request().params();

    try {
        const auto &records(callProcedure("ViewSaleReport", {
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
    } catch (DatabaseException &) {
        throw;
    }
}
