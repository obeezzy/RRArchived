#ifndef VIEWSALECART_H
#define VIEWSALECART_H

#include "saleexecutor.h"

namespace Query {
namespace Sales {
class ViewSaleCart : public SaleExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_sale_cart");
    explicit ViewSaleCart(int transactionId, QObject* receiver);
    QueryResult execute() override;
};
}  // namespace Sales
}  // namespace Query

#endif  // VIEWSALECART_H
