#ifndef STOCKPRODUCTCATEGORY_H
#define STOCKPRODUCTCATEGORY_H

#include "utility/common/note.h"
#include <QString>
#include <QVariantList>

namespace Utility {
    namespace Stock {
    struct ProductCategory
    {
        int id {0};
        QString category;
        Note note;
        int row {-1};

        explicit ProductCategory() = default;
        explicit ProductCategory(const QVariantMap &map);
        explicit ProductCategory(const QString &category,
                                  const Note &note = Note());
        explicit ProductCategory(int id,
                                  const QString &category,
                                  const Note &note = Note());

        QVariantMap toVariantMap() const;

        friend QDebug operator<<(QDebug debug, const ProductCategory &productCategory)
        {
            debug.nospace() << "StockProductCategory("
                            << "id=" << productCategory.id
                            << ", category=" << productCategory.category
                            << ")";

            return debug.nospace();
        }
    };

    class ProductCategoryList : public QList<ProductCategory>
    {
    public:
        explicit ProductCategoryList() = default;
        ProductCategoryList(std::initializer_list<ProductCategory> categories) :
            QList<ProductCategory>(categories) { }
        explicit ProductCategoryList(const QVariantList &list) :
            QList<ProductCategory>() {
            for (const auto &variant : list)
                append(ProductCategory{ variant.toMap() });
        }

        QVariantList toVariantList() const {
            QVariantList list;
            for (const auto &productCategory : *this)
                list.append(productCategory.toVariantMap());
            return list;
        }
    };
}
}
Q_DECLARE_TYPEINFO(Utility::Stock::ProductCategory, Q_PRIMITIVE_TYPE);

#endif // STOCKPRODUCTCATEGORY_H
