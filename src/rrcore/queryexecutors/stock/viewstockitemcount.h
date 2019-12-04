#ifndef VIEWSTOCKITEMCOUNT_H
#define VIEWSTOCKITEMCOUNT_H

#include "stockexecutor.h"

namespace StockQuery {
class ViewStockItemCount : public StockExecutor
{
public:
    static inline const QString COMMAND = QStringLiteral("view_stock_item_count");

    explicit ViewStockItemCount(int categoryId,
                                QObject *receiver);
    QueryResult execute() override;
};
}

#endif // VIEWSTOCKITEMCOUNT_H
