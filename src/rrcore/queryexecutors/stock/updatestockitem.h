#ifndef UPDATESTOCKITEM_H
#define UPDATESTOCKITEM_H

#include "stockexecutor.h"

namespace StockQuery {
class UpdateStockItem : public StockExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("update_stock_item");

    explicit UpdateStockItem(int itemId,
                             const QString &category,
                             const QString &item,
                             const QString &description,
                             const QString &unit,
                             bool tracked,
                             bool divisible,
                             qreal costPrice,
                             qreal retailPrice,
                             qreal baseUnitEquivalent,
                             bool isPreferredUnit,
                             const QString &currency,
                             const QUrl &imageUrl,
                             const QString &categoryNote,
                             const QString &itemNote,
                             QObject *receiver);
    QueryResult execute() override;
};
}

Q_DECLARE_LOGGING_CATEGORY(updateStockItem);

#endif // UPDATESTOCKITEM_H
