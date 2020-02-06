#ifndef STOCKPRODUCTCATEGORY_H
#define STOCKPRODUCTCATEGORY_H

#include "utility/commonutils.h"
#include <QString>

struct StockProductCategory
{
    int id;
    QString category;
    Note note;

    explicit StockProductCategory();
    explicit StockProductCategory(const QString &category,
                                  const Note &note = Note{});
    explicit StockProductCategory(int id,
                                  const QString &category,
                                  const Note &note = Note{});
};

#endif // STOCKPRODUCTCATEGORY_H
