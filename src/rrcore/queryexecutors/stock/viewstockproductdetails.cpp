#include "viewstockproductdetails.h"
#include "database/databaseexception.h"
#include "database/databaseutils.h"
#include "database/exceptions/exceptions.h"

#include <QUrl>

using namespace StockQuery;

ViewStockProductDetails::ViewStockProductDetails(int productId, QObject *receiver) :
    StockExecutor(COMMAND, {
                    { "product", productId }
                  }, receiver)
{

}

QueryResult ViewStockProductDetails::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);
    const QVariantMap &params = request().params();

    try {
        enforceArguments({ "product_id" }, params);

        const QList<QSqlRecord> &records(callProcedure("ViewStockProductDetails", {
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "product",
                                                               params.value("product_id")
                                                           }
                                                       }));

        QVariantMap product;
        if (!records.isEmpty()) {
            product = recordToMap(records.first());
            product.insert("image_url", DatabaseUtils::byteArrayToImageUrl(product.value("image").toByteArray()));
            product.remove("image");
        }
        else
            throw NoExistingRecordException(QStringLiteral("Product details do not exists for product '%1'.")
                                            .arg(params.value("product_id").toString()));

        result.setOutcome(QVariantMap {
                              { "product", product },
                              { "record_count", 1 }
                          });
        return result;
    } catch (DatabaseException &) {
        throw;
    }
}
