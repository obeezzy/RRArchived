#ifndef PRODUCT_H
#define PRODUCT_H

#include <QDateTime>
#include <QVariantList>
#include "productcategory.h"
#include "productquantity.h"
#include "productunit.h"
#include "singletons/settings.h"
#include "stockmonies.h"
#include "utility/common/money.h"
#include "utility/common/note.h"
#include "utility/common/recordtimestamp.h"
#include "utility/user/user.h"

namespace Utility {
namespace Stock {
struct Product
{
    int id{0};
    ProductCategory category;
    QString product;
    QString description;
    QUrl imageUrl;
    ProductQuantity quantity;
    ProductUnit unit;
    RecordGroup::Flags flags{RecordGroup::Tracked | RecordGroup::Divisible};
    StockMonies monies;
    Note note;
    RecordTimestamp timestamp;
    User::User user;
    int row{-1};

    explicit Product() = default;
    explicit Product(int id, const QString& product = QString(),
                     const Note& note = Note());
    explicit Product(const ProductCategory& category, const QString& product,
                     const QString& description, const QUrl& imageUrl,
                     const ProductQuantity& quantity, const ProductUnit& unit,
                     const RecordGroup::Flags& flags, const StockMonies& monies,
                     const Note& note = Note());
    explicit Product(int id, const ProductCategory& category,
                     const QString& product, const QString& description,
                     const QUrl& imageUrl, const ProductUnit& unit,
                     const RecordGroup::Flags& flags, const StockMonies& monies,
                     const Note& note = Note());
    explicit Product(int id, const ProductCategory& category,
                     const ProductQuantity& quantity, const ProductUnit& unit,
                     const StockMonies& monies, const Note& note = Note());
    explicit Product(const QVariantMap& map);
    QVariantMap toVariantMap() const;

    inline bool operator==(const Product& other) { return id == other.id; }

    friend QDebug operator<<(QDebug debug, const Product& product)
    {
        debug.nospace() << "Product("
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
                        << ", note=" << product.note << ", row=" << product.row
                        << ")";

        return debug.nospace();
    }
};

class ProductList : public QList<Product>
{
public:
    explicit ProductList() {}
    explicit ProductList(std::initializer_list<Product> productList)
        : QList<Product>(productList)
    {}
    explicit ProductList(const QVariantList& list) : QList<Product>()
    {
        for (const auto& variant : list)
            append(Product{variant.toMap()});
    }

    inline QVariantList toVariantList() const
    {
        QVariantList list;
        for (const auto& product : *this)
            list.append(product.toVariantMap());
        return list;
    }
};
}  // namespace Stock
}  // namespace Utility
Q_DECLARE_TYPEINFO(Utility::Stock::Product, Q_PRIMITIVE_TYPE);

#endif  // PRODUCT_H
