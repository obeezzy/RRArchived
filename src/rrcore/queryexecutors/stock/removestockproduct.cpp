#include "removestockproduct.h"
#include "database/databaseutils.h"
#include "database/databaseexception.h"
#include "database/exceptions/exceptions.h"
#include "user/userprofile.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>

using namespace StockQuery;

RemoveStockProduct::RemoveStockProduct(int productId,
                                       QObject *receiver) :
    StockExecutor(COMMAND, {
                    { "can_undo", true },
                    { "product_id", productId }
                  }, receiver)
{

}

RemoveStockProduct::RemoveStockProduct(int productRow,
                                       const StockProduct &product,
                                       QObject *receiver) :
    StockExecutor(COMMAND, {
                    { "can_undo", true },
                    { "product_id", product.id },
                    { "product_row", productRow },
                    { "product", product.toVariantMap() },
                    { "user_id", UserProfile::instance().userId() }
                  }, receiver)
{

}

QueryResult RemoveStockProduct::execute()
{
    if (canUndo() && isUndoSet())
        return undoRemoveStockProduct();

    return removeStockProduct();
}

QueryResult RemoveStockProduct::removeStockProduct()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request().params();
    QSqlQuery q(connection);

    try {
        QueryExecutor::enforceArguments({ "product_id" }, params);

        DatabaseUtils::beginTransaction(q);

        callProcedure("ArchiveStockProduct", {
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "product_id",
                              params.value("product_id")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "user_id",
                              params.value("user_id")
                          }
                      });

        DatabaseUtils::commitTransaction(q);
        return result;
    } catch (DatabaseException &) {
        DatabaseUtils::rollbackTransaction(q);
        throw;
    }
}

QueryResult RemoveStockProduct::undoRemoveStockProduct()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request().params();
    QSqlQuery q(connection);

    try {
        QueryExecutor::enforceArguments({ "product_id" }, params);

        DatabaseUtils::beginTransaction(q);

        const QList<QSqlRecord> &records(callProcedure("UndoArchiveStockProduct", {
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "product_id",
                                                               params.value("product_id")
                                                           },
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "user_id",
                                                               UserProfile::instance().userId()
                                                           }
                                                       }));

        if (records.isEmpty())
            throw UnexpectedResultException("No results returned.");

        QVariantMap outcome;
        result.setOutcome(QVariantMap {
                              { "product_category_id", records.first().value("product_category_id").toInt() },
                              { "category", records.first().value("category").toString() },
                              { "product_id", records.first().value("product_id").toInt() },
                              { "product", recordToMap(records.first()) },
                              { "product_row", params.value("product_row").toInt() }
                          });

        DatabaseUtils::commitTransaction(q);
        return result;
    } catch (DatabaseException &) {
        DatabaseUtils::rollbackTransaction(q);
        throw;
    }
}
