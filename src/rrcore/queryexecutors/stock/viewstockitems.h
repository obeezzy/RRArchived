#ifndef VIEWSTOCKITEMS_H
#define VIEWSTOCKITEMS_H

#include "stockexecutor.h"

namespace StockQuery {
class ViewStockItems : public StockExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_stock_items");

    explicit ViewStockItems(int categoryId,
                            Qt::SortOrder sortOrder,
                            QObject *receiver);
    QueryResult execute() override;
};
}

#endif // VIEWSTOCKITEMS_H
