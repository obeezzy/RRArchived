#include "filterstockitems.h"
#include "database/databaseexception.h"
#include "database/databaseutils.h"

#include <QUrl>

using namespace StockQuery;

FilterStockItems::FilterStockItems(int categoryId,
                                   const QString &filterText,
                                   const QString &filterColumn,
                                   Qt::SortOrder sortOrder,
                                   const QString &sortColumn,
                                   QObject *receiver) :
    StockExecutor(COMMAND, {
                        { "category_id", categoryId },
                        { "filter_text", filterText },
                        { "filter_column", filterColumn },
                        { "sort_order", sortOrder },
                        { "sort_column", sortColumn }
                  }, receiver)
{

}

QueryResult FilterStockItems::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);
    const QVariantMap &params = request().params();

    try {
        const QList<QSqlRecord> &records(callProcedure("FilterStockItems", {
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "category_id",
                                                               params.value("category_id")
                                                           },
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
                                                               "sort_order",
                                                               params.value("sort_order")
                                                           },
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "sort_column",
                                                               params.value("sort_column")
                                                           }
                                                       }));

        QVariantList items;
        for (const auto &record : records) {
            QVariantMap itemRecord { recordToMap(record) };
            itemRecord.insert("image_url", DatabaseUtils::byteArrayToImageUrl(record.value("image").toByteArray()));
            itemRecord.remove("image");

            items.append(itemRecord);
        }

        result.setOutcome(QVariantMap {
                              { "items", items },
                              { "record_count", items.count() }
                          });
        return result;
    } catch (DatabaseException &) {
        throw;
    }
}
