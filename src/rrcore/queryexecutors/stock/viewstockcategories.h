#ifndef VIEWSTOCKCATEGORIES_H
#define VIEWSTOCKCATEGORIES_H

#include "stockexecutor.h"

namespace StockQuery {
class ViewStockCategories : public StockExecutor
{
public:
    static inline const QString COMMAND = QStringLiteral("view_stock_categories");

    explicit ViewStockCategories(Qt::SortOrder sortOrder,
                                 bool archived,
                                 QObject *receiver);
    QueryResult execute() override;
};
}

#endif // VIEWSTOCKCATEGORIES_H
