#ifndef PURCHASECARTPRODUCT_H
#define PURCHASECARTPRODUCT_H

#include "utility/purchase/purchasemonies.h"
#include "utility/common/money.h"
#include "utility/common/note.h"
#include "utility/common/recordtimestamp.h"
#include "utility/stock/productcategory.h"
#include "utility/stock/productquantity.h"
#include "utility/stock/productunit.h"
#include "utility/user/user.h"
#include "singletons/settings.h"
#include <QVariantList>
#include <QDateTime>

namespace Utility {
    namespace Purchase {
    struct PurchaseCartProduct {
        int id {0};
        QString product;
        Stock::ProductCategory category;
        QString description;
        QUrl imageUrl;
        Stock::ProductQuantity quantity;
        Stock::ProductQuantity availableQuantity;
        Stock::ProductUnit unit;
        PurchaseMonies monies;
        RecordGroup::Flags flags;
        Note note;
        RecordTimestamp timestamp;
        User::User user;

        explicit PurchaseCartProduct() = default;
        explicit PurchaseCartProduct(int id);
        explicit PurchaseCartProduct(const QVariantMap &map);
        explicit PurchaseCartProduct(const QString &product,
                                     const Stock::ProductCategory &category,
                                     const QString &description,
                                     const QUrl &imageUrl,
                                     const Stock::ProductQuantity &quantity,
                                     const Stock::ProductUnit &unit,
                                     const PurchaseMonies &monies,
                                     const RecordGroup::Flags &flags,
                                     const Note &note = Note());
        explicit PurchaseCartProduct(int id,
                                     const QString &product,
                                     const Stock::ProductCategory &category,
                                     const Stock::ProductQuantity &quantity,
                                     const Stock::ProductUnit &unit,
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
}
Q_DECLARE_TYPEINFO(Utility::Purchase::PurchaseCartProduct, Q_PRIMITIVE_TYPE);

#endif // PURCHASECARTPRODUCT_H
