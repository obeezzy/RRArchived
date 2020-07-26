#ifndef FILTERSTOCKPRODUCTS_H
#define FILTERSTOCKPRODUCTS_H

#include "stockexecutor.h"

namespace Utility {
class FilterCriteria;
class SortCriteria;
}  // namespace Utility

namespace Query {
namespace Stock {
class FilterProducts : public StockExecutor
{
    Q_OBJECT
public:
    static inline QString COMMAND = QStringLiteral("filter_products");

    explicit FilterProducts(const Utility::FilterCriteria& filterCriteria,
                            const Utility::SortCriteria& sortCriteria,
                            int productCategoryId, QObject* receiver);
    QueryResult execute() override;
};
}  // namespace Stock
}  // namespace Query

#endif  // FILTERSTOCKPRODUCTS_H
