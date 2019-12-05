#ifndef ADDSTOCKITEM_H
#define ADDSTOCKITEM_H

#include "stockexecutor.h"

namespace StockQuery {
class AddStockItem : public StockExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("add_stock_item");

    explicit AddStockItem(const QString &category,
                          const QString &item,
                          const QString &description,
                          double quantity,
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

private:
};
}

#endif // ADDSTOCKITEM_H
