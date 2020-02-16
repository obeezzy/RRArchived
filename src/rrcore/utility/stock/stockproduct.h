#ifndef STOCKPRODUCT_H
#define STOCKPRODUCT_H

#include "utility/common/note.h"
#include "utility/user/user.h"
#include "stockproductcategory.h"
#include "stockproductunit.h"
#include <QVariantList>
#include <QDateTime>

namespace Utility {
struct StockProduct {
    int id {-1};
    StockProductCategory category;
    QString product;
    QString description;
    QUrl imageUrl;
    double quantity {0.0};
    StockProductUnit unit;
    RecordGroup::Flags flags{ RecordGroup::Tracked | RecordGroup::Divisible };
    qreal costPrice {0.0};
    qreal retailPrice {0.0};
    qreal unitPrice {0.0};
    QString currency;
    Note note;
    QDateTime created;
    QDateTime lastEdited;
    User user;
    int row {-1};

    explicit StockProduct() = default;
    explicit StockProduct(int id);
    explicit StockProduct(const StockProductCategory &category,
                          const QString &product,
                          const QString &description,
                          const QUrl &imageUrl,
                          double quantity,
                          const StockProductUnit &unit,
                          const RecordGroup::Flags &flags,
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
                          const RecordGroup::Flags &flags,
                          qreal costPrice,
                          qreal retailPrice,
                          const QString &currency,
                          const Note &note);
    explicit StockProduct(int id,
                          const StockProductCategory &category,
                          qreal quantity,
                          const StockProductUnit &unit,
                          qreal retailPrice,
                          qreal unitPrice,
                          const Note &note);
    explicit StockProduct(const QVariantMap &product);
    QVariantMap toVariantMap() const;

    inline bool operator==(const StockProduct &other) {
        return id == other.id;
    }
};

class StockProductList : public QList<StockProduct> {
public:
    explicit StockProductList() {}
    explicit StockProductList(std::initializer_list<StockProduct> productList) :
        QList<StockProduct>(productList) {}
    explicit StockProductList(const QVariantList &list) :
        QList<StockProduct>() {
        for (const auto &variant : list)
            append(StockProduct{ variant.toMap() });
    }

    inline QVariantList toVariantList() const {
        QVariantList list;
        for (const auto &stockProduct : *this)
            list.append(stockProduct.toVariantMap());
        return list;
    }
};
}

#endif // STOCKPRODUCT_H
