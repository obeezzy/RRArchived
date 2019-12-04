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
                    { "item_id", itemId },
                    { "can_undo", true }
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
