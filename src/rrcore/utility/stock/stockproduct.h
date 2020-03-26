#ifndef STOCKPRODUCT_H
#define STOCKPRODUCT_H

#include "utility/common/note.h"
#include "utility/user/user.h"
#include "stockproductcategory.h"
#include "stockproductunit.h"
#include "singletons/settings.h"
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
    QString currency {Settings::DEFAULT_CURRENCY};
    Note note;
    QDateTime created;
    QDateTime lastEdited;
    User user;
    int row {-1};

    explicit StockProduct() = default;
    explicit StockProduct(int id,
                          const QString &product = QString(),
                          const Note &note = Note());
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

    friend QDebug operator<<(QDebug debug, const StockProduct &product)
    {
        debug.nospace() << "StockProduct("
                        << "id=" << product.id
                        << ", productCategory=" << product.category
                        << ", product=" << product.product
                        << ", description=" << product.description
                        << ", imageUrl=" << product.imageUrl
                        << ", quantity=" << product.quantity
                        << ", unit=" << product.unit
                        << ", flags=" << product.flags
                        << ", costPrice=" << product.costPrice
                        << ", retailPrice=" << product.retailPrice
                        << ", unitPrice=" << product.unitPrice
                        << ", note=" << product.note
                        << ", row=" << product.row
                        << ")";

        return debug.nospace();
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
        for (const auto &product : *this)
            list.append(product.toVariantMap());
        return list;
    }
};
}
Q_DECLARE_TYPEINFO(Utility::StockProduct, Q_PRIMITIVE_TYPE);

#endif // STOCKPRODUCT_H
