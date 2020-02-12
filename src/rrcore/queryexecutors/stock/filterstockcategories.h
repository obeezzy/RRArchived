#ifndef FILTERSTOCKCATEGORIES_H
#define FILTERSTOCKCATEGORIES_H

#include "stockexecutor.h"
#include "utility/commonutils.h"

namespace StockQuery {
class FilterStockCategories : public StockExecutor
{
public:
    static inline const QString COMMAND = QStringLiteral("filter_stock_categories");

    explicit FilterStockCategories(const FilterCriteria &filterCriteria,
                                   const SortCriteria &sortCriteria,
                                   const RecordGroup::Flags &RecordGroups,
                                   QObject *receiver);
    QueryResult execute() override;
};
}

#endif // FILTERSTOCKCATEGORIES_H
