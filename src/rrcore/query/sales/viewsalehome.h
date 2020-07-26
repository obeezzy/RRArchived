#ifndef VIEWSALEHOME_H
#define VIEWSALEHOME_H

#include "saleexecutor.h"

namespace Query {
namespace Sales {
class ViewSaleHome : public SaleExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_sale_home");

    explicit ViewSaleHome(QObject* receiver);
    QueryResult execute() override;

private:
    void fetchTotalRevenue(QVariantList& homeRecords);
    void fetchMostSoldProducts(QVariantList& homeRecords);
};
}  // namespace Sales
}  // namespace Query

#endif  // VIEWSALEHOME_H
