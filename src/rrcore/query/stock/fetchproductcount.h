#ifndef FETCHPRODUCTCOUNT_H
#define FETCHPRODUCTCOUNT_H

#include "stockexecutor.h"

namespace Query {
namespace Stock {
class FetchProductCount : public StockExecutor
{
public:
    static inline const QString COMMAND = QStringLiteral("fetch_product_count");

    explicit FetchProductCount(int categoryId, QObject* receiver);
    QueryResult execute() override;
};
}  // namespace Stock
}  // namespace Query

#endif  // FETCHPRODUCTCOUNT_H
