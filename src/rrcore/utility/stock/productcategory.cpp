#include "productcategory.h"

using namespace Utility::Stock;

ProductCategory::ProductCategory(const QVariantMap &map) :
    id(map.value("product_category_id").toInt()),
    category(map.value("product_category").toString()),
    note(Note{ map })
{
}

ProductCategory::ProductCategory(const QString &category,
                                   const Note &note) :
    category(category),
    note(note)
{}

ProductCategory::ProductCategory(int id,
                                   const QString &category,
                                   const Note &note) :
    id(id),
    category(category),
    note(note)
{}

QVariantMap ProductCategory::toVariantMap() const
{
    return {
        { "product_category_id", id },
        { "product_category", category },
        { "note_id", note.id },
        { "note", note.note }
    };
}
