#ifndef STOCKPRODUCTCATEGORY_H
#define STOCKPRODUCTCATEGORY_H

#include "utility/commonutils.h"
#include <QString>
#include <QVariantList>

namespace Utility {
struct StockProductCategory
{
    int id {0};
    QString category;
    Note note;
    int row {-1};

    explicit StockProductCategory() = default;
    explicit StockProductCategory(const QVariantMap &map);
    explicit StockProductCategory(const QString &category,
                                  const Note &note = Note{});
    explicit StockProductCategory(int id,
                                  const QString &category,
                                  const Note &note = Note{});

    QVariantMap toVariantMap() const;

    friend QDebug operator<<(QDebug debug, const StockProductCategory &productCategory)
    {
        debug.nospace() << "StockProductCategory("
                        << "id=" << productCategory.id
                        << ", category=" << productCategory.category
                        << ")";

        return debug.nospace();
    }
};

class StockProductCategoryList : public QList<StockProductCategory>
{
public:
    explicit StockProductCategoryList() = default;
    StockProductCategoryList(std::initializer_list<StockProductCategory> categories) :
        QList<StockProductCategory>(categories) { }
    explicit StockProductCategoryList(const QVariantList &list) :
        QList<StockProductCategory>() {
        for (const auto &variant : list)
            append(StockProductCategory{ variant.toMap() });
    }

    QVariantList toVariantList() const {
        QVariantList list;
        for (const auto &productCategory : *this)
            list.append(productCategory.toVariantMap());
        return list;
    }
};
}
Q_DECLARE_TYPEINFO(Utility::StockProductCategory, Q_PRIMITIVE_TYPE);

#endif // STOCKPRODUCTCATEGORY_H
