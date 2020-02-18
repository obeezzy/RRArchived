#ifndef FILTERSTOCKCATEGORIES_H
#define FILTERSTOCKCATEGORIES_H

#include "stockexecutor.h"
#include "utility/common/recordgroup.h"

namespace Utility {
class FilterCriteria;
class SortCriteria;
}

namespace StockQuery {
class FilterStockCategories : public StockExecutor
{
public:
    static inline const QString COMMAND = QStringLiteral("filter_stock_categories");

    explicit FilterStockCategories(const Utility::FilterCriteria &filterCriteria,
                                   const Utility::SortCriteria &sortCriteria,
                                   const Utility::RecordGroup::Flags &flags,
                                   QObject *receiver);
    QueryResult execute() override;
};
}

#endif // FILTERSTOCKCATEGORIES_H
