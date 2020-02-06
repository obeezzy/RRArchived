#ifndef FILTERSTOCKPRODUCTS_H
#define FILTERSTOCKPRODUCTS_H

#include "stockexecutor.h"

namespace StockQuery {
class FilterStockProducts : public StockExecutor
{
    Q_OBJECT
public:
    static inline QString COMMAND = QStringLiteral("filter_stock_products");

    explicit FilterStockProducts(int categoryId,
                                 const QString &filterText,
                                 const QString &filterColumn,
                                 Qt::SortOrder sortOrder,
                                 const QString &sortColumn,
                                 QObject *receiver);
    QueryResult execute() override;
};
}

#endif // FILTERSTOCKPRODUCTS_H
