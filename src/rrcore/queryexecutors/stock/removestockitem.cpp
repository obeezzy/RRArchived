#include "removestockitem.h"

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
                    { "item_info", item.toVariantMap() }
                  }, receiver)
{

}

QueryResult RemoveStockItem::execute()
{
    QueryResult result{ request() };

    return result;
}
