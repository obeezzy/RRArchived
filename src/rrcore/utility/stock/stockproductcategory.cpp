#include "stockproductcategory.h"

using namespace Utility;

StockProductCategory::StockProductCategory(const QVariantMap &map)
{
    Q_UNUSED(map)
}

StockProductCategory::StockProductCategory(const QString &category,
                                           const Note &note) :
    category(category),
    note(note)
{}

StockProductCategory::StockProductCategory(int id,
                                           const QString &category,
                                           const Note &note) :
    id(id),
    category(category),
    note(note)
{}

QVariantMap StockProductCategory::toVariantMap() const
{
    return {};
}
