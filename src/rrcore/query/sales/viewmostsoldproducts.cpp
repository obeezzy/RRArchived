#include "viewmostsoldproducts.h"
#include "database/exceptions/exceptions.h"
#include "user/userprofile.h"
#include "utility/common/datetimespan.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

const int LIMIT = 5;

using namespace Query::Sales;

ViewMostSoldProducts::ViewMostSoldProducts(const Utility::DateTimeSpan &dateTimeSpan,
                                           QObject *receiver) :
    SaleExecutor(COMMAND, {
                    { "from", dateTimeSpan.from },
                    { "to", dateTimeSpan.to }
                 }, receiver)
{}

QueryResult ViewMostSoldProducts::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    const QVariantMap &params{ request().params() };

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QSqlQuery q(connection);

    try {
        const auto &records(callProcedure("ViewMostSoldProducts", {
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
                                                  "limit",
                                                  params.value("limit", LIMIT)
                                              }
                                          }));

        QVariantList products;
        for (const auto &record : records)
            products.append(recordToMap(record));

        if (!products.isEmpty()) {
            result.setOutcome(QVariantMap {
                                  { "products", products },
                                  { "record_count", products.count() }
                              });
        }

        return result;
    } catch (const DatabaseException &) {
        throw;
    }
}
