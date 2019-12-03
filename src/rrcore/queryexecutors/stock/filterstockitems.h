#ifndef FILTERSTOCKITEMS_H
#define FILTERSTOCKITEMS_H

#include "stockexecutor.h"

namespace StockQuery {
class FilterStockItems : public StockExecutor
{
    Q_OBJECT
public:
    static inline QString COMMAND = QStringLiteral("filter_stock_items");

    explicit FilterStockItems(int categoryId,
                              const QString &filterText,
                              const QString &filterColumn,
                              Qt::SortOrder sortOrder,
                              const QString &sortColumn,
                              QObject *receiver);
    QueryResult execute() override;
};
}

#endif // FILTERSTOCKITEMS_H
