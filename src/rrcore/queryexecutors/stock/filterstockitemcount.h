#ifndef FILTERSTOCKITEMCOUNT_H
#define FILTERSTOCKITEMCOUNT_H

#include "stockexecutor.h"

namespace StockQuery {
class FilterStockItemCount : public StockExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("filter_stock_item_count");

    FilterStockItemCount(int categoryId,
                         const QString &filterText,
                         const QString &filterColumn,
                         QObject *receiver);
    QueryResult execute() override;
};
}

#endif // FILTERSTOCKITEMCOUNT_H
