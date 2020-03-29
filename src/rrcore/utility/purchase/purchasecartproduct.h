#ifndef PURCHASECARTPRODUCT_H
#define PURCHASECARTPRODUCT_H

#include "purchasemonies.h"
#include "utility/stock/stockproductquantity.h"
#include "utility/common/note.h"
#include "utility/common/recordtimestamp.h"
#include "utility/stock/stockproductcategory.h"
#include "utility/stock/stockproductunit.h"
#include "utility/user/user.h"
#include "singletons/settings.h"
#include <QVariantList>
#include <QDateTime>

namespace Utility {
struct PurchaseCartProduct {
    int id {0};
    QString product;
    StockProductCategory category;
    QString description;
    QUrl imageUrl;
    StockProductQuantity quantity;
    StockProductQuantity availableQuantity;
    StockProductUnit unit;
    PurchaseMonies monies;
    RecordGroup::Flags flags;
    Note note;
    RecordTimestamp timestamp;
    User user;

    explicit PurchaseCartProduct() = default;
    explicit PurchaseCartProduct(int id);
    explicit PurchaseCartProduct(const QVariantMap &map);
    explicit PurchaseCartProduct(const QString &product,
                                 const StockProductCategory &category,
                                 const QString &description,
                                 const QUrl &imageUrl,
                                 const StockProductQuantity &quantity,
                                 const StockProductUnit &unit,
                                 const PurchaseMonies &monies,
                                 const RecordGroup::Flags &flags,
                                 const Note &note = Note());
    explicit PurchaseCartProduct(int id,
                                 const QString &product,
                                 const StockProductCategory &category,
                                 const StockProductQuantity &quantity,
                                 const StockProductUnit &unit,
                                 const PurchaseMonies &monies,
                                 const Note &note = Note());
    QVariantMap toVariantMap() const;

    inline bool operator==(const PurchaseCartProduct &other) {
        return id == other.id;
    }
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
}
Q_DECLARE_TYPEINFO(Utility::PurchaseCartProduct, Q_PRIMITIVE_TYPE);

#endif // PURCHASECARTPRODUCT_H
