#ifndef FILTERSTOCKCATEGORIESBYITEM_H
#define FILTERSTOCKCATEGORIESBYITEM_H

#include "stockexecutor.h"

namespace StockQuery {
class FilterStockCategoriesByItem : public StockExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("filter_stock_categories_by_item");

    explicit FilterStockCategoriesByItem(const QString &itemFilterText,
                                         Qt::SortOrder sortOrder,
                                         bool archived,
                                         QObject *receiver);
    QueryResult execute() override;
};
}

#endif // FILTERSTOCKCATEGORIESBYITEM_H
