#ifndef FILTERSTOCKPRODUCTCATEGORIESBYPRODUCT_H
#define FILTERSTOCKPRODUCTCATEGORIESBYPRODUCT_H

#include "stockexecutor.h"
#include "utility/common/recordgroup.h"

namespace Utility {
class FilterCriteria;
class SortCriteria;
}

namespace StockQuery {
class FilterStockProductCategoriesByProduct : public StockExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("filter_stock_product_categories_by_product");

    explicit FilterStockProductCategoriesByProduct(const Utility::FilterCriteria &filterCriteria,
                                                   const Utility::SortCriteria &sortCriteria,
                                                   const Utility::RecordGroup::Flags &flags,
                                                   QObject *receiver);
    QueryResult execute() override;
};
}

#endif // FILTERSTOCKPRODUCTCATEGORIESBYPRODUCT_H
