#ifndef STOCKREPORTTRANSACTION_H
#define STOCKREPORTTRANSACTION_H

#include <QVariantList>
#include "utility/stock/product.h"
#include "utility/stock/productcategory.h"

namespace Utility {
namespace Stock {
struct StockReportTransaction
{
    ProductCategory category;
    Product product;
    ProductQuantity openingStockQuantity;
    ProductQuantity quantitySold;
    ProductQuantity quantityBought;
    ProductQuantity quantityInStock;

    explicit StockReportTransaction() = default;
    explicit StockReportTransaction(const QVariantMap& map);

    QVariantMap toVariantMap() const;
};

class StockReportTransactionList : public QList<StockReportTransaction>
{
public:
    explicit StockReportTransactionList() = default;
    StockReportTransactionList(
        std::initializer_list<StockReportTransaction> transactions)
        : QList<StockReportTransaction>(transactions)
    {}
    explicit StockReportTransactionList(const QVariantList& list)
        : QList<StockReportTransaction>()
    {
        for (const auto& variant : list)
            append(StockReportTransaction{variant.toMap()});
    }

    QVariantList toVariantList() const
    {
        QVariantList list;
        for (const auto& transaction : *this)
            list.append(transaction.toVariantMap());
        return list;
    }
};
}  // namespace Stock
}  // namespace Utility
Q_DECLARE_TYPEINFO(Utility::Stock::StockReportTransaction, Q_PRIMITIVE_TYPE);

#endif  // STOCKREPORTTRANSACTION_H
