#include "viewpurchasereport.h"
#include "database/databaseexception.h"
#include "utility/purchaseutils.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

using namespace PurchaseQuery;

ViewPurchaseReport::ViewPurchaseReport(const Utility::DateTimeSpan &dateTimeSpan,
                                       QObject *receiver) :
    PurchaseExecutor(COMMAND, {
                        { "from", dateTimeSpan.from },
                        { "to", dateTimeSpan.to }
                     }, receiver)
{

}

QueryResult ViewPurchaseReport::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    const QVariantMap &params = request().params();

    try {
        const auto &records(callProcedure("ViewPurchaseReport", {
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
