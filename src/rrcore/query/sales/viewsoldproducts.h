#ifndef VIEWSOLDPRODUCTS_H
#define VIEWSOLDPRODUCTS_H

#include "saleexecutor.h"

namespace Query {
namespace Sales {
class ViewSoldProducts : public SaleExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_sold_products");

    explicit ViewSoldProducts(int transactionId, QObject* receiver);
    QueryResult execute() override;
};
}  // namespace Sales
}  // namespace Query

#endif  // VIEWSOLDPRODUCTS_H
