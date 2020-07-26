#include "viewproducts.h"
#include "database/databaseexception.h"
#include "utility/common/sortcriteria.h"
#include <QUrl>

using namespace Query::Stock;

ViewProducts::ViewProducts(int productCategoryId,
                                     const Utility::SortCriteria &sortCriteria,
                                     QObject *receiver) :
    StockExecutor(COMMAND, {
                    { "product_category_id", productCategoryId },
                    { "sort_order", sortCriteria.orderAsString() }
                  }, receiver)
{

}

QueryResult ViewProducts::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    const QVariantMap &params{ request().params() };

    try {
        const auto &records(callProcedure("ViewProducts", {
                                              ProcedureArgument {
                                                  ProcedureArgument::Type::In,
                                                  "product_category_id",
                                                  params.value("product_category_id")
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
            product.insert("image_url", QueryExecutor::byteArrayToImageUrl(record.value("image").toByteArray()));
            product.remove("image");
            products.append(product);
        }

        result.setOutcome(QVariantMap {
                              { "products", products },
                              { "record_count", products.count() }
                          });
        return result;
    } catch (const DatabaseException &) {
        throw;
    }
}
