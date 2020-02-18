#ifndef VIEWSTOCKPRODUCTCATEGORIES_H
#define VIEWSTOCKPRODUCTCATEGORIES_H

#include "stockexecutor.h"
#include "utility/common/recordgroup.h"

namespace Utility {
class SortCriteria;
}

namespace StockQuery {
class ViewStockProductCategories : public StockExecutor
{
public:
    static inline const QString COMMAND = QStringLiteral("view_stock_product_categories");

    explicit ViewStockProductCategories(const Utility::SortCriteria &sortCriteria,
                                        const Utility::RecordGroup::Flags &flags,
                                        QObject *receiver);
    QueryResult execute() override;
};
}

#endif // VIEWSTOCKPRODUCTCATEGORIES_H
