#ifndef UPDATESTOCKPRODUCT_H
#define UPDATESTOCKPRODUCT_H

#include "stockexecutor.h"

namespace StockQuery {
class UpdateStockProduct : public StockExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("update_stock_product");

    explicit UpdateStockProduct(int productId,
                                const QString &category,
                                const QString &product,
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
                                const QString &productNote,
                                QObject *receiver);
    QueryResult execute() override;
};
}

Q_DECLARE_LOGGING_CATEGORY(lcupdatestockproduct);

#endif // UPDATESTOCKPRODUCT_H
