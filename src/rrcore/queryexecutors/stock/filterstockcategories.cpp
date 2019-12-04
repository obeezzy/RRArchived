#include "filterstockcategories.h"
#include "database/databaseexception.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

using namespace StockQuery;

FilterStockCategories::FilterStockCategories(const QString &filterText,
                                             Qt::SortOrder sortOrder,
                                             bool archived,
                                             QObject *receiver) :
    StockExecutor(COMMAND, {
                    { "filter_text", filterText },
                    { "sort_order", sortOrder == Qt::DescendingOrder ? "descending" : "ascending" },
                    { "archived", archived }
                  }, receiver)
{

}

QueryResult FilterStockCategories::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);
    const QVariantMap &params(request().params());

    try {
        const QList<QSqlRecord> &records(callProcedure("FilterStockCategories", {
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "filter_text",
                                                               params.value("filter_text")
                                                           },
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "sort_order",
                                                               params.value("sort_order")
                                                           }
                                                       }));

        QVector<int> categoryIds;
        QVariantList categories;
        for (const auto &record : records) {
            if (!categoryIds.contains(record.value("category_id").toInt())) {
                categories.append(recordToMap(record));
                categoryIds.append(record.value("category_id").toInt());
            }
        }

        result.setOutcome(QVariantMap {
                              { "categories", categories },
                              { "record_count", categories.count() }
                          });
        return result;
    } catch (DatabaseException &) {
        throw;
    }
}
