#ifndef VIEWPRODUCTS_H
#define VIEWPRODUCTS_H

#include "stockexecutor.h"

namespace Utility {
class SortCriteria;
}

namespace StockQuery {
class ViewProducts : public StockExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_products");

    explicit ViewProducts(int productCategoryId,
                               const Utility::SortCriteria &sortCriteria,
                               QObject *receiver);
    QueryResult execute() override;
};
}

#endif // VIEWPRODUCTS_H
