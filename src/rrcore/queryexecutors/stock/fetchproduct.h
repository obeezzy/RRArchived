#ifndef FETCHPRODUCT_H
#define FETCHPRODUCT_H

#include "stockexecutor.h"

namespace StockQuery {
class FetchProduct : public StockExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("fetch_product");

    explicit FetchProduct(int productId,
                               QObject *receiver);
    QueryResult execute() override;
};
}

#endif // FETCHPRODUCT_H
