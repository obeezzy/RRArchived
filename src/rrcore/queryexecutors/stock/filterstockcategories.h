#ifndef FILTERSTOCKCATEGORIES_H
#define FILTERSTOCKCATEGORIES_H

#include "stockexecutor.h"

namespace StockQuery {
class FilterStockCategories : public StockExecutor
{
public:
    static inline const QString COMMAND = QStringLiteral("filter_stock_categories");

    explicit FilterStockCategories(const QString &filterText,
                                   Qt::SortOrder sortOrder,
                                   bool archived,
                                   QObject *receiver);
    QueryResult execute() override;
};
}

#endif // FILTERSTOCKCATEGORIES_H
