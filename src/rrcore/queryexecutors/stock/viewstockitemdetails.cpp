#include "viewstockitemdetails.h"
#include "database/databaseexception.h"
#include "database/databaseutils.h"

#include <QUrl>

using namespace StockQuery;

ViewStockItemDetails::ViewStockItemDetails(int itemId, QObject *receiver) :
    StockExecutor(COMMAND, {
                        { "item_id", itemId }
                  }, receiver)
{

}

QueryResult ViewStockItemDetails::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);
    const QVariantMap &params = request().params();

    try {
        enforceArguments({ "item_id" }, params);

        const QList<QSqlRecord> &records(callProcedure("ViewStockItemDetails", {
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "item",
                                                               params.value("item_id")
                                                           }
                                                       }));

        QVariantMap itemInfo;
        if (!records.isEmpty()) {
            itemInfo = recordToMap(records.first());
            itemInfo.insert("image_url", DatabaseUtils::byteArrayToImageUrl(itemInfo.value("image").toByteArray()));
            itemInfo.remove("image");
        }
        else
            throw DatabaseException(DatabaseError::QueryErrorCode::ViewStockItemDetailsFailed,
                                    QString(),
                                    QString("Item details do not exists for item '%1'.").arg(params.value("item_id").toString()));

        result.setOutcome(QVariantMap {
                              { "item", itemInfo },
                              { "record_count", 1 }
                          });
        return result;
    } catch (DatabaseException &) {
        throw;
    }
}
