#include "stockproductcategory.h"

using namespace Utility;

StockProductCategory::StockProductCategory() :
    id(-1)
{

}

StockProductCategory::StockProductCategory(const QString &category,
                                           const Note &note) :
    id(-1),
    category(category),
    note(note)
{ }

StockProductCategory::StockProductCategory(int id,
                                           const QString &category,
                                           const Note &note) :
    id(id),
    category(category),
    note(note)
{

}
