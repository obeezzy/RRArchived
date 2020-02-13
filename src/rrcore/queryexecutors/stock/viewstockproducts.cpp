#include "viewstockproducts.h"
#include "database/databaseexception.h"
#include "database/databaseutils.h"
#include "utility/commonutils.h"
#include <QUrl>

using namespace StockQuery;

ViewStockProducts::ViewStockProducts(int productCategoryId,
                                     const Utility::SortCriteria &sortCriteria,
                                     QObject *receiver) :
    StockExecutor(COMMAND, {
                        { "category_id", productCategoryId },
                        { "sort_order", sortCriteria.orderAsString() }
                  }, receiver)
{

}

QueryResult ViewStockProducts::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    const QVariantMap &params = request().params();

    try {
        const QList<QSqlRecord> &records(callProcedure("ViewStockProducts", {
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

        QVariantList products;
        for (const auto &record : records) {
            QVariantMap product{ recordToMap(record) };
            product.insert("image_url", DatabaseUtils::byteArrayToImageUrl(record.value("image").toByteArray()));
            product.remove("image");
            products.append(product);
        }

        result.setOutcome(QVariantMap {
                              { "products", products },
                              { "record_count", products.count() }
                          });
        return result;
    } catch (DatabaseException &) {
        throw;
    }
}
