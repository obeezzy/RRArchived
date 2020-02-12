#ifndef STOCKPRODUCT_H
#define STOCKPRODUCT_H

#include "utility/user/user.h"
#include "utility/commonutils.h"
#include "stockproductcategory.h"
#include "stockproductunit.h"

#include <QList>
#include <QVariantList>
#include <QVariantMap>
#include <QDateTime>

struct StockProduct {
    int id;
    StockProductCategory category;
    QString product;
    QString description;
    QUrl imageUrl;
    double quantity;
    StockProductUnit unit;
    bool tracked;
    bool divisible;
    qreal costPrice;
    qreal retailPrice;
    qreal unitPrice;
    QString currency;
    Note note;
    QDateTime created;
    QDateTime lastEdited;
    User user;
    int row;

    explicit StockProduct(int id);
    explicit StockProduct(const StockProductCategory &category,
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
    explicit StockProduct(int id,
                          const StockProductCategory &category,
                          const QString &product,
                          const QString &description,
                          const QUrl &imageUrl,
                          const StockProductUnit &unit,
                          bool tracked,
                          bool divisible,
                          qreal costPrice,
                          qreal retailPrice,
                          const QString &currency,
                          const Note &note);
    explicit StockProduct(int id,
                          const StockProductCategory &category,
                          double quantity,
                          const StockProductUnit &unit,
                          qreal retailPrice,
                          qreal unitPrice,
                          const Note &note);
    explicit StockProduct(const QVariantMap &product);
    QVariantMap toVariantMap() const;
};

class StockProductList : public QList<StockProduct> {
public:
    explicit StockProductList() {}
    explicit StockProductList(std::initializer_list<StockProduct> productList) :
        QList<StockProduct>(productList) {}
    explicit StockProductList(const QVariantList &list) :
        QList<StockProduct>() {
        for (const auto &variant : list)
            append(StockProduct{variant.toMap()});
    }

    inline QVariantList toVariantList() const {
        QVariantList list;
        for (const auto &stockProduct : *this)
            list.append(stockProduct.toVariantMap());
        return list;
    }
};

#endif // STOCKPRODUCT_H
