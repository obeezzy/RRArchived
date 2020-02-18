#ifndef VIEWSTOCKPRODUCTS_H
#define VIEWSTOCKPRODUCTS_H

#include "stockexecutor.h"

namespace Utility {
class SortCriteria;
}

namespace StockQuery {
class ViewStockProducts : public StockExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_stock_products");

    explicit ViewStockProducts(int productCategoryId,
                               const Utility::SortCriteria &sortCriteria,
                               QObject *receiver);
    QueryResult execute() override;
};
}

#endif // VIEWSTOCKPRODUCTS_H
