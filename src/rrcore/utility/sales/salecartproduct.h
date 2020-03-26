#ifndef SALECARTPRODUCT_H
#define SALECARTPRODUCT_H

#include "utility/user/user.h"
#include "utility/common/note.h"
#include "utility/stock/stockproductcategory.h"
#include "utility/stock/stockproductunit.h"
#include "singletons/settings.h"
#include <QVariantList>
#include <QDateTime>

namespace Utility {
struct SaleCartProduct {
    int id {-1};
    QString product;
    StockProductCategory category;
    QString description;
    QUrl imageUrl;
    qreal quantity {0.0};
    qreal availableQuantity {0.0};
    StockProductUnit unit;
    RecordGroup::Flags flags;
    qreal costPrice {0.0};
    qreal retailPrice {0.0};
    qreal unitPrice {0.0};
    qreal cost {0.0};
    qreal amountPaid {0.0};
    QString currency {Settings::DEFAULT_CURRENCY};
    Note note;
    QDateTime created;
    QDateTime lastEdited;
    User user;

    explicit SaleCartProduct(int id);
    explicit SaleCartProduct(const StockProductCategory &category,
                             const QString &product,
                             const QString &description,
                             const QUrl &imageUrl,
                             qreal quantity,
                             const StockProductUnit &unit,
                             const RecordGroup::Flags &flags,
                             qreal costPrice,
                             qreal retailPrice,
                             const QString &currency,
                             const Note &note);
    explicit SaleCartProduct(int id,
                             const QString &product,
                             const StockProductCategory &category,
                             qreal quantity,
                             const StockProductUnit &unit,
                             qreal retailPrice,
                             qreal unitPrice,
                             qreal cost,
                             qreal amountPaid,
                             const Note &note);
    explicit SaleCartProduct(const QVariantMap &product);
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
                        << ", costPrice=" << product.costPrice
                        << ", retailPrice=" << product.retailPrice
                        << ", unitPrice=" << product.unitPrice
                        << ", cost=" << product.cost
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
