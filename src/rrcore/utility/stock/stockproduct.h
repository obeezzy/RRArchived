#ifndef STOCKPRODUCT_H
#define STOCKPRODUCT_H

#include "stockmonies.h"
#include "stockproductcategory.h"
#include "stockproductquantity.h"
#include "stockproductunit.h"
#include "singletons/settings.h"
#include "utility/common/money.h"
#include "utility/common/note.h"
#include "utility/common/recordtimestamp.h"
#include "utility/user/user.h"
#include <QDateTime>
#include <QVariantList>

namespace Utility {
struct StockProduct {
    int id {0};
    StockProductCategory category;
    QString product;
    QString description;
    QUrl imageUrl;
    StockProductQuantity quantity;
    StockProductUnit unit;
    RecordGroup::Flags flags{ RecordGroup::Tracked | RecordGroup::Divisible };
    StockMonies monies;
    Note note;
    RecordTimestamp timestamp;
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
                          const StockProductQuantity &quantity,
                          const StockProductUnit &unit,
                          const RecordGroup::Flags &flags,
                          const StockMonies &monies,
                          const Note &note = Note());
    explicit StockProduct(int id,
                          const StockProductCategory &category,
                          const QString &product,
                          const QString &description,
                          const QUrl &imageUrl,
                          const StockProductUnit &unit,
                          const RecordGroup::Flags &flags,
                          const StockMonies &monies,
                          const Note &note = Note());
    explicit StockProduct(int id,
                          const StockProductCategory &category,
                          const StockProductQuantity &quantity,
                          const StockProductUnit &unit,
                          const StockMonies &monies,
                          const Note &note = Note());
    explicit StockProduct(const QVariantMap &map);
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
                        << ", costPrice=" << product.monies.costPrice
                        << ", retailPrice=" << product.monies.retailPrice
                        << ", unitPrice=" << product.monies.unitPrice
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
