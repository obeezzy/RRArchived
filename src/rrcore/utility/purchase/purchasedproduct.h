#ifndef PURCHASEDPRODUCT_H
#define PURCHASEDPRODUCT_H

#include "utility/stock/stockproductcategory.h"
#include "utility/stock/stockproduct.h"
#include "utility/common/note.h"
#include "utility/common/recordgroup.h"
#include "utility/user/user.h"
#include "utility/purchase/purchasetransaction.h"
#include <QVariantList>

namespace Utility {
struct PurchasedProduct
{
    int id {-1};
    PurchaseTransaction transaction;
    StockProductCategory category;
    StockProduct product;
    qreal cost {0.0};
    qreal discount {0.0};
    Note note;
    QDateTime created;
    QDateTime lastEdited;
    User user;
    RecordGroup::Flags flags;
    int row {-1};

    explicit PurchasedProduct() = default;
    explicit PurchasedProduct(int id,
                              int purchaseTransactionId);
    explicit PurchasedProduct(const QVariantMap &map);

    QVariantMap toVariantMap() const;
};

class PurchasedProductList : public QList<PurchasedProduct>
{
public:
    explicit PurchasedProductList() = default;
    PurchasedProductList(std::initializer_list<PurchasedProduct> products) :
        QList<PurchasedProduct>(products) { }
    explicit PurchasedProductList(const QVariantList &list) :
        QList<PurchasedProduct>() {
        for (const auto &variant : list)
            append(PurchasedProduct{ variant.toMap() });
    }

    QVariantList toVariantList() const {
        QVariantList list;
        for (const auto &product : *this)
            list.append(product.toVariantMap());
        return list;
    }
};
} Q_DECLARE_TYPEINFO(Utility::PurchasedProduct, Q_PRIMITIVE_TYPE);

#endif // PURCHASEDPRODUCT_H
