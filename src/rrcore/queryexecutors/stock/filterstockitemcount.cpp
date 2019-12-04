#include "filterstockitemcount.h"
#include "database/databaseexception.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

using namespace StockQuery;

FilterStockItemCount::FilterStockItemCount(int categoryId,
                                           const QString &filterText,
                                           const QString &filterColumn,
                                           QObject *receiver) :
    StockExecutor(COMMAND, {
                        { "category_id", categoryId },
                        { "filter_text", filterText },
                        { "filter_column", filterColumn }
                  }, receiver)
{

}

QueryResult FilterStockItemCount::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);
    const QVariantMap &params = request().params();

    try {
        const QList<QSqlRecord> &records(callProcedure("FilterStockItemCount", {
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "filter_text",
                                                               params.value("filter_text")
                                                           },
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "filter_column",
                                                               params.value("filter_column")
                                                           },
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "archived",
                                                               params.value("archived")
                                                           }
                                                       }));

        int itemCount = 0;
        if (!records.isEmpty())
            itemCount = records.first().value("item_count").toInt();

        result.setOutcome(QVariantMap {
                              { "item_count", itemCount },
                              { "record_count", 1 }
                          });
        return result;
    } catch (DatabaseException &) {
        throw;
    }
}
