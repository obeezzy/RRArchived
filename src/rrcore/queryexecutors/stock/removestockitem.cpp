#include "removestockitem.h"
#include "database/databaseutils.h"
#include "database/databaseexception.h"
#include "user/userprofile.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>

using namespace StockQuery;

RemoveStockItem::RemoveStockItem(int itemId, QObject *receiver) :
    StockExecutor(COMMAND, {
                    { "can_undo", true },
                    { "item_id", itemId }
                  }, receiver)
{

}

RemoveStockItem::RemoveStockItem(int itemId, int itemRow, StockItem item, QObject *receiver) :
    StockExecutor(COMMAND, {
                    { "can_undo", true },
                    { "item_id", itemId },
                    { "item_row", itemRow },
                    { "item_info", item.toVariantMap() },
                    { "user_id", UserProfile::instance().userId() }
                  }, receiver)
{

}

QueryResult RemoveStockItem::execute()
{
    if (canUndo() && isUndoSet())
        return undoRemoveStockItem();

    return removeStockItem();
}

QueryResult RemoveStockItem::removeStockItem()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request().params();
    QSqlQuery q(connection);

    try {
        QueryExecutor::enforceArguments({ "item_id" }, params);

        DatabaseUtils::beginTransaction(q);

        callProcedure("ArchiveStockItem", {
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "item_id",
                              params.value("item_id")
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

QueryResult RemoveStockItem::undoRemoveStockItem()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request().params();
    QSqlQuery q(connection);

    try {
        QueryExecutor::enforceArguments({ "item_id" }, params);

        DatabaseUtils::beginTransaction(q);

        const QList<QSqlRecord> &records(callProcedure("UndoArchiveStockItem", {
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "item_id",
                                                               params.value("item_id")
                                                           },
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "user_id",
                                                               UserProfile::instance().userId()
                                                           }
                                                       }));

        if (records.isEmpty())
            throw DatabaseException(DatabaseError::QueryErrorCode::EmptyResultSet,
                                    QString(),
                                    "No results returned.");

        QVariantMap outcome;
        result.setOutcome(QVariantMap {
                              { "category_id", records.first().value("category_id").toInt() },
                              { "category", records.first().value("category").toString() },
                              { "item_id", records.first().value("item_id").toInt() },
                              { "item_info", recordToMap(records.first()) },
                              { "item_row", params.value("item_row").toInt() }
                          });

        DatabaseUtils::commitTransaction(q);
        return result;
    } catch (DatabaseException &) {
        DatabaseUtils::rollbackTransaction(q);
        throw;
    }
}
