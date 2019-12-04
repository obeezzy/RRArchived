#ifndef VIEWSTOCKITEMDETAILS_H
#define VIEWSTOCKITEMDETAILS_H

#include "stockexecutor.h"

namespace StockQuery {
class ViewStockItemDetails : public StockExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_stock_item_details");

    explicit ViewStockItemDetails(int itemId,
                                  QObject *receiver);
    QueryResult execute() override;
};
}

#endif // VIEWSTOCKITEMDETAILS_H
