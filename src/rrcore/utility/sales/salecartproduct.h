#ifndef SALECARTPRODUCT_H
#define SALECARTPRODUCT_H

#include "utility/user/user.h"
#include "utility/commonutils.h"
#include "utility/stockutils.h"
#include <QVariantList>
#include <QList>
#include <QVariantList>
#include <QVariantMap>
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
    QString currency;
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

#endif // SALECARTPRODUCT_H
