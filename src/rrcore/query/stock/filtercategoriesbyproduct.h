#ifndef FILTERCATEGORIESBYPRODUCT_H
#define FILTERCATEGORIESBYPRODUCT_H

#include "stockexecutor.h"
#include "utility/common/recordgroup.h"

namespace Utility {
class FilterCriteria;
class SortCriteria;
}  // namespace Utility

namespace Query {
namespace Stock {
class FilterCategoriesByProduct : public StockExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND =
        QStringLiteral("filter_categories_by_product");

    explicit FilterCategoriesByProduct(
        const Utility::FilterCriteria& filterCriteria,
        const Utility::SortCriteria& sortCriteria,
        const Utility::RecordGroup::Flags& flags, QObject* receiver);
    QueryResult execute() override;
};
}  // namespace Stock
}  // namespace Query

#endif  // FILTERCATEGORIESBYPRODUCT_H
