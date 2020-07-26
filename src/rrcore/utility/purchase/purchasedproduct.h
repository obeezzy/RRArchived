#ifndef PURCHASEDPRODUCT_H
#define PURCHASEDPRODUCT_H

#include <QVariantList>
#include "utility/common/note.h"
#include "utility/common/recordgroup.h"
#include "utility/common/recordtimestamp.h"
#include "utility/purchase/purchasetransaction.h"
#include "utility/stock/product.h"
#include "utility/stock/productcategory.h"
#include "utility/user/user.h"

namespace Utility {
namespace Purchase {
struct PurchasedProduct
{
    int id{0};
    PurchaseTransaction transaction;
    Stock::ProductCategory category;
    Stock::Product product;
    PurchaseMonies monies;
    RecordGroup::Flags flags;
    Note note;
    RecordTimestamp timestamp;
    User::User user;
    int row{-1};

    explicit PurchasedProduct() = default;
    explicit PurchasedProduct(int id, int purchaseTransactionId);
    explicit PurchasedProduct(const QVariantMap& map);

    QVariantMap toVariantMap() const;
};

class PurchasedProductList : public QList<PurchasedProduct>
{
public:
    explicit PurchasedProductList() = default;
    PurchasedProductList(std::initializer_list<PurchasedProduct> products)
        : QList<PurchasedProduct>(products)
    {}
    explicit PurchasedProductList(const QVariantList& list)
        : QList<PurchasedProduct>()
    {
        for (const auto& variant : list)
            append(PurchasedProduct{variant.toMap()});
    }

    QVariantList toVariantList() const
    {
        QVariantList list;
        for (const auto& product : *this)
            list.append(product.toVariantMap());
        return list;
    }
};
}  // namespace Purchase
}  // namespace Utility
Q_DECLARE_TYPEINFO(Utility::Purchase::PurchasedProduct, Q_PRIMITIVE_TYPE);

#endif  // PURCHASEDPRODUCT_H
