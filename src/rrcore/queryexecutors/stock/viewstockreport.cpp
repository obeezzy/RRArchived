#include "viewstockreport.h"
#include "database/databaseexception.h"

using namespace StockQuery;

ViewStockReport::ViewStockReport(QObject *receiver) :
    StockExecutor(COMMAND, {
                    { "sort_column", QStringLiteral("category") }
                  }, receiver)
{

}

QueryResult ViewStockReport::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);
    const QVariantMap &params = request().params();

    try {
        const QList<QSqlRecord> &records(callProcedure("ViewStockReport", {
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
                                                           }
                                                       }));

        QVariantList products;
        for (const QSqlRecord &record : records) {
            products.append(recordToMap(record));
        }

        result.setOutcome(QVariantMap { { "products", products },
                                        { "record_count", products.count() },
                          });
        return result;
    } catch (DatabaseException &) {
        throw;
    }
}
