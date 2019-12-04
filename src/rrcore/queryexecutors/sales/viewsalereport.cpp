#include "viewsalereport.h"
#include "database/databaseexception.h"

using namespace SaleQuery;

ViewSaleReport::ViewSaleReport(QObject *receiver) :
    SaleExecutor(COMMAND, {
                    { "sort_column", QStringLiteral("category") },
                    { "sort_order", "ascending" }
                 }, receiver)
{

}

QueryResult ViewSaleReport::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);
    const QVariantMap &params = request().params();

    try {
        const QList<QSqlRecord> &records(callProcedure("ViewSaleReport", {
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
                                                               params.value("sort_order").toInt() == Qt::DescendingOrder
                                                               ? "descending" : "ascending"
                                                           }
                                                       }));

        QVariantList items;
        for (const QSqlRecord &record : records) {
            items.append(recordToMap(record));
        }

        result.setOutcome(QVariantMap {
                              { "items", items },
                              { "record_count", items.count() },
                          });
        return result;
    } catch (DatabaseException &) {
        throw;
    }
}
