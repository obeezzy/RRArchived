#include "viewstockcategories.h"
#include "database/databaseexception.h"

using namespace StockQuery;

ViewStockCategories::ViewStockCategories(Qt::SortOrder sortOrder,
                                         bool archived,
                                         QObject *receiver) :
    StockExecutor(COMMAND, {
                    { "sort_order", sortOrder == Qt::DescendingOrder ? "descending" : "ascending" },
                    { "archived", archived }
                  }, receiver)
{

}

QueryResult ViewStockCategories::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);
    const QVariantMap &params(request().params());

    try {
        const QList<QSqlRecord> &records(callProcedure("ViewStockCategories", {
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "sort_order",
                                                               params.value("sort_order", "ascending")
                                                           },
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "archived",
                                                               params.value("archived", false)
                                                           }
                                                       }));

        QVariantList categories;
        for (const auto &record : records) {
            categories.append(recordToMap(record));
        }

        result.setOutcome(QVariantMap {
                              { "categories", categories },
                              { "record_count", categories.count() }
                          });
    } catch (DatabaseException &) {
        throw;
    }
    return result;
}
