#ifndef VIEWSALEHOME_H
#define VIEWSALEHOME_H

#include "saleexecutor.h"

class DateTimeSpan;

namespace SaleQuery {
class ViewSaleHome : public SaleExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_sale_home");

    explicit ViewSaleHome(const DateTimeSpan &dateTimeSpan,
                          QObject *receiver);
    QueryResult execute() override;
private:
    void fetchTotalRevenue(QVariantList &homeRecords);
    void fetchMostSoldProducts(QVariantList &homeRecords);
};
}

#endif // VIEWSALEHOME_H
