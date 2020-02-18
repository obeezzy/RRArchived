#include "fetchstockproduct.h"
#include "database/databaseexception.h"
#include "database/exceptions/exceptions.h"
#include <QUrl>

using namespace StockQuery;

FetchStockProduct::FetchStockProduct(int productId,
                                     QObject *receiver) :
    StockExecutor(COMMAND, {
                    { "product_id", productId }
                  }, receiver)
{

}

QueryResult FetchStockProduct::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    const QVariantMap &params = request().params();

    try {
        enforceArguments({ "product_id" }, params);

        const auto &records(callProcedure("FetchStockProduct", {
                                              ProcedureArgument {
                                                  ProcedureArgument::Type::In,
                                                  "product_id",
                                                  params.value("product_id")
                                              }
                                          }));

        if (records.isEmpty())
            throw NoExistingRecordException(QStringLiteral("Product details do not exists for product '%1'.")
                                            .arg(params.value("product_id").toString()));

        QVariantMap product{ recordToMap(records.first()) };
        product.insert("image_url", QueryExecutor::byteArrayToImageUrl(product.value("image").toByteArray()));
        product.remove("image");
        result.setOutcome(QVariantMap {
                              { "product", product },
                              { "record_count", 1 }
                          });
        return result;
    } catch (const DatabaseException &) {
        throw;
    }
}
