#ifndef VIEWSOLDPRODUCTS_H
#define VIEWSOLDPRODUCTS_H

#include "saleexecutor.h"

namespace SaleQuery {
class ViewSoldProducts : public SaleExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_sold_products");

    explicit ViewSoldProducts(qint64 transactionId,
                              QObject *receiver);
    QueryResult execute() override;
};
}

#endif // VIEWSOLDPRODUCTS_H
