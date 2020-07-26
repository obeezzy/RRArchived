#ifndef SALECARTPRODUCT_H
#define SALECARTPRODUCT_H

#include "utility/common/note.h"
#include "utility/common/recordtimestamp.h"
#include "utility/sales/salemonies.h"
#include "utility/stock/productcategory.h"
#include "utility/stock/productunit.h"
#include "utility/stock/productquantity.h"
#include "utility/user/user.h"
#include <QVariantList>
#include <QDateTime>

namespace Utility {
    namespace Sales {
    struct SaleCartProduct {
        int id {0};
        QString product;
        Stock::ProductCategory category;
        QString description;
        QUrl imageUrl;
        Stock::ProductQuantity quantity;
        Stock::ProductQuantity availableQuantity;
        Stock::ProductUnit unit;
        SaleMonies monies;
        RecordGroup::Flags flags;
        Note note;
        RecordTimestamp timestamp;
        User::User user;

        explicit SaleCartProduct(int id);
        explicit SaleCartProduct(const QString &product,
                                 const Utility::Stock::ProductCategory &category,
                                 const QString &description,
                                 const QUrl &imageUrl,
                                 const Utility::Stock::ProductQuantity &quantity,
                                 const Utility::Stock::ProductUnit &unit,
                                 const SaleMonies &monies,
                                 const Utility::RecordGroup::Flags &flags,
                                 const Utility::Note &note = Note());
        explicit SaleCartProduct(int id,
                                 const QString &product,
                                 const Utility::Stock::ProductCategory &category,
                                 const Utility::Stock::ProductQuantity &quantity,
                                 const Utility::Stock::ProductUnit &unit,
                                 const SaleMonies &monies,
                                 const Utility::Note &note = Note());
        explicit SaleCartProduct(const QVariantMap &map);
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
                            << ", costPrice=" << product.monies.costPrice
                            << ", retailPrice=" << product.monies.retailPrice
                            << ", unitPrice=" << product.monies.unitPrice
                            << ", cost=" << product.monies.cost
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
}
Q_DECLARE_TYPEINFO(Utility::Sales::SaleCartProduct, Q_PRIMITIVE_TYPE);

#endif // SALECARTPRODUCT_H
