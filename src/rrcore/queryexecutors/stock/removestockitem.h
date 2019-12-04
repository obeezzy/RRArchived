#ifndef REMOVESTOCKITEM_H
#define REMOVESTOCKITEM_H

#include "stockexecutor.h"
#include "utility/stockutils.h"

namespace StockQuery {
class RemoveStockItem : public StockExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("remove_stock_item");
    static inline const QString UNDO_COMMAND = QStringLiteral("undo_remove_stock_item");

    explicit RemoveStockItem(int itemId, QObject *receiver);
    explicit RemoveStockItem(int itemId, int itemRow, StockItem item, QObject *receiver);
    QueryResult execute() override;
private:
    QueryResult removeStockItem();
    QueryResult undoRemoveStockItem();
};
}

#endif // REMOVESTOCKITEM_H
