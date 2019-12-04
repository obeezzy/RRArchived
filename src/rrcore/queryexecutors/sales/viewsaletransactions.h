#ifndef VIEWSALETRANSACTIONS_H
#define VIEWSALETRANSACTIONS_H

#include "saleexecutor.h"

namespace SaleQuery {
class ViewSaleTransactions : public SaleExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("view_sale_transactions");

    explicit ViewSaleTransactions(const QDateTime &from,
                                  const QDateTime &to,
                                  bool suspended,
                                  bool archived,
                                  QObject *receiver);
    QueryResult execute() override;
};
}

#endif // VIEWSALETRANSACTIONS_H
