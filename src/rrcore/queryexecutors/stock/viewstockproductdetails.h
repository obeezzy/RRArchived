#ifndef VIEWSTOCKPRODUCTDETAILS_H
#define VIEWSTOCKPRODUCTDETAILS_H

#include "stockexecutor.h"

namespace StockQuery {
class ViewStockProductDetails : public StockExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_stock_product_details");

    explicit ViewStockProductDetails(int productId,
                                     QObject *receiver);
    QueryResult execute() override;
};
}

#endif // VIEWSTOCKPRODUCTDETAILS_H
