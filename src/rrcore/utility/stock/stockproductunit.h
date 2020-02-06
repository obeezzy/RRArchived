#ifndef STOCKPRODUCTUNIT_H
#define STOCKPRODUCTUNIT_H

#include <QString>

struct StockProductUnit
{
    int id;
    QString unit;
    bool preferred;
    qreal baseUnitEquivalent;

    explicit StockProductUnit();
    explicit StockProductUnit(int id,
                              const QString &unit);
    explicit StockProductUnit(const QString &unit,
                              qreal baseUnitEquivalent);
};

#endif // STOCKPRODUCTUNIT_H
