#include "viewstockitems.h"
#include "database/databaseexception.h"
#include "database/databaseutils.h"

#include <QUrl>

using namespace StockQuery;

ViewStockItems::ViewStockItems(int categoryId,
                               Qt::SortOrder sortOrder,
                               QObject *receiver) :
    StockExecutor(COMMAND, {
                        { "category_id", categoryId },
                        { "sort_order", sortOrder == Qt::DescendingOrder ? "descending" : "ascending" }
                  }, receiver)
{

}

QueryResult ViewStockItems::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);
    const QVariantMap &params = request().params();

    try {
        const QList<QSqlRecord> &records(callProcedure("ViewStockItems", {
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "category_id",
                                                               params.value("category_id")
                                                           },
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "sort_order",
                                                               params.value("sort_order")
                                                           }
                                                       }));

        QVariantList items;
        for (const auto &record : records) {
            QVariantMap itemRecord{ recordToMap(record) };
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
