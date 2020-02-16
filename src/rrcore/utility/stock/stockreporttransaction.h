#ifndef STOCKREPORTTRANSACTION_H
#define STOCKREPORTTRANSACTION_H

#include "utility/stock/stockproductcategory.h"
#include "utility/stock/stockproduct.h"
#include <QVariantList>

namespace Utility {
struct StockReportTransaction
{
    StockProductCategory category;
    StockProduct product;
    qreal openingStockQuantity {0.0};
    qreal quantitySold {0.0};
    qreal quantityBought {0.0};
    qreal quantityInStock {0.0};

    explicit StockReportTransaction() = default;
    explicit StockReportTransaction(const QVariantMap &map);

    QVariantMap toVariantMap() const;
};

class StockReportTransactionList : public QList<StockReportTransaction>
{
public:
    explicit StockReportTransactionList() = default;
    StockReportTransactionList(std::initializer_list<StockReportTransaction> transactions) :
        QList<StockReportTransaction>(transactions) { }
    explicit StockReportTransactionList(const QVariantList &list) :
        QList<StockReportTransaction>() {
        for (const auto &variant : list)
            append(StockReportTransaction{ variant.toMap() });
    }

    QVariantList toVariantList() const {
        QVariantList list;
        for (const auto &transaction : *this)
            list.append(transaction.toVariantMap());
        return list;
    }
};
} Q_DECLARE_TYPEINFO(Utility::StockReportTransaction, Q_PRIMITIVE_TYPE);

#endif // STOCKREPORTTRANSACTION_H
