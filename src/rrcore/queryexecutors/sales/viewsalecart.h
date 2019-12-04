#ifndef VIEWSALECART_H
#define VIEWSALECART_H

#include "saleexecutor.h"

namespace SaleQuery {
class ViewSaleCart : public SaleExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_sale_cart");

    explicit ViewSaleCart(qint64 transactionId, QObject *receiver);
    QueryResult execute() override;
};
}

#endif // VIEWSALECART_H
