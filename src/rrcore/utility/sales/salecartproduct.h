#ifndef SALECARTPRODUCT_H
#define SALECARTPRODUCT_H

#include <QVariantList>

#include "utility/user/user.h"
#include "utility/commonutils.h"
#include "utility/stockutils.h"

#include <QList>
#include <QVariantList>
#include <QVariantMap>
#include <QDateTime>

struct SaleCartProduct {
    int id;
    QString product;
    StockProductCategory category;
    QString description;
    QUrl imageUrl;
    double quantity;
    StockProductUnit unit;
    bool tracked;
    bool divisible;
    qreal costPrice;
    qreal retailPrice;
    qreal unitPrice;
    qreal cost;
    qreal amountPaid;
    QString currency;
    Note note;
    QDateTime created;
    QDateTime lastEdited;
    User user;

    explicit SaleCartProduct(const StockProductCategory &category,
                             const QString &product,
                             const QString &description,
                             const QUrl &imageUrl,
                             double quantity,
                             const StockProductUnit &unit,
                             bool tracked,
                             bool divisible,
                             qreal costPrice,
                             qreal retailPrice,
                             const QString &currency,
                             const Note &note);
    explicit SaleCartProduct(int id,
                             const QString &product,
                             const StockProductCategory &category,
                             double quantity,
                             const StockProductUnit &unit,
                             qreal retailPrice,
                             qreal unitPrice,
                             qreal cost,
                             qreal amountPaid,
                             const Note &note);
    explicit SaleCartProduct(const QVariantMap &product);
    QVariantMap toVariantMap() const;
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

#endif // SALECARTPRODUCT_H
