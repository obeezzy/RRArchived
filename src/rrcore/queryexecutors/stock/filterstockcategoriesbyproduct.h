#ifndef FILTERSTOCKPRODUCTCATEGORIESBYPRODUCT_H
#define FILTERSTOCKPRODUCTCATEGORIESBYPRODUCT_H

#include "stockexecutor.h"
#include "utility/commonutils.h"

namespace StockQuery {
class FilterStockProductCategoriesByProduct : public StockExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("filter_stock_product_categories_by_product");

    explicit FilterStockProductCategoriesByProduct(const FilterCriteria &filterCriteria,
                                                   const SortCriteria &sortCriteria,
                                                   const RecordGroup::Flags &flags,
                                                   QObject *receiver);
    QueryResult execute() override;
};
}

#endif // FILTERSTOCKPRODUCTCATEGORIESBYPRODUCT_H
