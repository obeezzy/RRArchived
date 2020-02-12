#ifndef VIEWSTOCKPRODUCTCATEGORIES_H
#define VIEWSTOCKPRODUCTCATEGORIES_H

#include "stockexecutor.h"
#include "utility/stockutils.h"

namespace StockQuery {
class ViewStockProductCategories : public StockExecutor
{
public:
    static inline const QString COMMAND = QStringLiteral("view_stock_product_categories");

    explicit ViewStockProductCategories(const SortCriteria &sortCriteria,
                                        const RecordGroup::Flags &flags,
                                        QObject *receiver);
    QueryResult execute() override;
};
}

#endif // VIEWSTOCKPRODUCTCATEGORIES_H
