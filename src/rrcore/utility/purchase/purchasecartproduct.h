#ifndef PURCHASECARTPRODUCT_H
#define PURCHASECARTPRODUCT_H

#include "utility/user/user.h"
#include "utility/commonutils.h"
#include "utility/stockutils.h"
#include <QVariantList>
#include <QList>
#include <QVariantList>
#include <QVariantMap>
#include <QDateTime>

struct PurchaseCartProduct {
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

    explicit PurchaseCartProduct(const StockProductCategory &category,
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
    explicit PurchaseCartProduct(int id,
                             const QString &product,
                             const StockProductCategory &category,
                             double quantity,
                             const StockProductUnit &unit,
                             qreal retailPrice,
                             qreal unitPrice,
                             qreal cost,
                             qreal amountPaid,
                             const Note &note);
    explicit PurchaseCartProduct(const QVariantMap &product);
    QVariantMap toVariantMap() const;
};

class PurchaseCartProductList : public QList<PurchaseCartProduct> {
public:
    explicit PurchaseCartProductList() {}
    explicit PurchaseCartProductList(std::initializer_list<PurchaseCartProduct> productList) :
        QList<PurchaseCartProduct>(productList) {}
    explicit PurchaseCartProductList(const QVariantList &list) :
        QList<PurchaseCartProduct>() {
        for (const auto &variant : list)
            append(PurchaseCartProduct{variant.toMap()});
    }

    inline QVariantList toVariantList() const {
        QVariantList list;
        for (const auto &stockProduct : *this)
            list.append(stockProduct.toVariantMap());
        return list;
    }
};

#endif // PURCHASECARTPRODUCT_H
