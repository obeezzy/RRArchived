#include "removeproduct.h"
#include "database/databaseexception.h"
#include "database/exceptions/exceptions.h"
#include "user/userprofile.h"
#include "utility/stock/product.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>

using namespace Query::Stock;

RemoveProduct::RemoveProduct(const Utility::Stock::Product &product,
                                       QObject *receiver) :
    StockExecutor(COMMAND, {
                    { "can_undo", true },
                    { "product_id", product.id },
                    { "row", product.row },
                    { "product", product.toVariantMap() },
                    { "user_id", UserProfile::instance().userId() }
                  }, receiver)
{}

RemoveProduct::RemoveProduct(const QueryRequest &request,
                                       QObject *receiver) :
    StockExecutor(COMMAND, request.params(), receiver)
{}

QueryResult RemoveProduct::execute()
{
    if (canUndo() && isUndoSet())
        return undoRemoveProduct();

    return removeProduct();
}

QueryResult RemoveProduct::removeProduct()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request().params();
    QSqlQuery q(connection);

    try {
        QueryExecutor::enforceArguments({ "product_id" }, params);

        QueryExecutor::beginTransaction(q);

        callProcedure("ArchiveProduct", {
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
                      });

        QueryExecutor::commitTransaction(q);
        return result;
    } catch (const DatabaseException &) {
        QueryExecutor::rollbackTransaction(q);
        throw;
    }
}

QueryResult RemoveProduct::undoRemoveProduct()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params{ request().params() };
    QSqlQuery q(connection);

    try {
        QueryExecutor::enforceArguments({ "product_id" }, params);

        QueryExecutor::beginTransaction(q);

        const auto &records(callProcedure("UndoArchiveProduct", {
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
            throw UnexpectedResultException("Expected archived product, received nothing.");

        QVariantMap outcome;
        result.setOutcome(QVariantMap {
                              { "product_category_id", records.first().value("product_category_id").toInt() },
                              { "category", records.first().value("category").toString() },
                              { "product_id", records.first().value("product_id").toInt() },
                              { "product", recordToMap(records.first()) },
                              { "row", params.value("row").toInt() }
                          });

        QueryExecutor::commitTransaction(q);
        return result;
    } catch (const DatabaseException &) {
        QueryExecutor::rollbackTransaction(q);
        throw;
    }
}
