#include "stockproductcategory.h"

using namespace Utility;

StockProductCategory::StockProductCategory(const QVariantMap &map) :
    id(map.value("product_category_id").toInt()),
    category(map.value("product_category").toString()),
    note(Note{ map })
{
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
    return {
        { "product_category_id", id },
        { "product_category", category },
        { "note_id", note.id },
        { "note", note.note }
    };
}
