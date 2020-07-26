#ifndef VIEWMOSTSOLDPRODUCTS_H
#define VIEWMOSTSOLDPRODUCTS_H

#include <QObject>
#include "saleexecutor.h"
#include "utility/common/datetimespan.h"

namespace Query {
namespace Sales {
class ViewMostSoldProducts : public SaleExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND =
        QStringLiteral("view_most_sold_products");

    explicit ViewMostSoldProducts(const Utility::DateTimeSpan& dateTimeSpan,
                                  QObject* receiver);

    QueryResult execute() override;
};
}  // namespace Sales
}  // namespace Query

#endif  // VIEWMOSTSOLDPRODUCTS_H
