#ifndef VIEWSALEHOME_H
#define VIEWSALEHOME_H

#include "saleexecutor.h"

namespace SaleQuery {
class ViewSaleHome : public SaleExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_sale_home");

    explicit ViewSaleHome(QObject *receiver);
    QueryResult execute() override;
};
}

#endif // VIEWSALEHOME_H
