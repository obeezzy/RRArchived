#ifndef SALECARTPRODUCT_H
#define SALECARTPRODUCT_H

#include "utility/common/note.h"
#include "utility/common/recordtimestamp.h"
#include "utility/sales/salemonies.h"
#include "utility/stock/stockproductcategory.h"
#include "utility/stock/stockproductunit.h"
#include "utility/stock/stockproductquantity.h"
#include "utility/user/user.h"
#include <QVariantList>
#include <QDateTime>

namespace Utility {
struct SaleCartProduct {
    int id {0};
    QString product;
    StockProductCategory category;
    QString description;
    QUrl imageUrl;
    StockProductQuantity quantity;
    StockProductQuantity availableQuantity;
    StockProductUnit unit;
    SaleMonies monies;
    RecordGroup::Flags flags;
    Note note;
    RecordTimestamp timestamp;
    User user;

    explicit SaleCartProduct(int id);
    explicit SaleCartProduct(const QString &product,
                             const StockProductCategory &category,
                             const QString &description,
                             const QUrl &imageUrl,
                             const StockProductQuantity &quantity,
                             const StockProductUnit &unit,
                             const SaleMonies &monies,
                             const RecordGroup::Flags &flags,
                             const Note &note = Note());
    explicit SaleCartProduct(int id,
                             const QString &product,
                             const StockProductCategory &category,
                             const StockProductQuantity &quantity,
                             const StockProductUnit &unit,
                             const SaleMonies &monies,
                             const Note &note = Note());
    explicit SaleCartProduct(const QVariantMap &map);
    QVariantMap toVariantMap() const;

    inline bool operator==(const SaleCartProduct &other) {
        return id == other.id;
    }

    friend QDebug operator<<(QDebug debug, const SaleCartProduct &product)
    {
        debug.nospace() << "SaleCartProduct("
                        << "id=" << product.id
                        << ", product=" << product.product
                        << ", caetegory=" << product.category
                        << ", description=" << product.description
                        << ", imageUrl=" << product.imageUrl
                        << ", quantity=" << product.quantity
                        << ", availableQuantity=" << product.availableQuantity
                        << ", unit=" << product.unit
                        << ", costPrice=" << product.monies.costPrice
                        << ", retailPrice=" << product.monies.retailPrice
                        << ", unitPrice=" << product.monies.unitPrice
                        << ", cost=" << product.monies.cost
                        << ", note=" << product.note
                        << ")";

        return debug.nospace();
    }
};

class SaleCartProductList : public QList<SaleCartProduct> {
public:
    explicit SaleCartProductList() {}
    explicit SaleCartProductList(std::initializer_list<SaleCartProduct> productList) :
        QList<SaleCartProduct>(productList) {}
    explicit SaleCartProductList(const QVariantList &list) :
        QList<SaleCartProduct>() {
        for (const auto &variant : list)
            append(SaleCartProduct{variant.toMap()});
    }

    inline QVariantList toVariantList() const {
        QVariantList list;
        for (const auto &stockProduct : *this)
            list.append(stockProduct.toVariantMap());
        return list;
    }
};
}
Q_DECLARE_TYPEINFO(Utility::SaleCartProduct, Q_PRIMITIVE_TYPE);

#endif // SALECARTPRODUCT_H
