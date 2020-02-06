#include "stockproductunit.h"

StockProductUnit::StockProductUnit() :
    id(-1),
    preferred(false),
    baseUnitEquivalent(1.0)
{

}

StockProductUnit::StockProductUnit(int id,
                                   const QString &unit) :
    id(id),
    unit(unit)
{
}

StockProductUnit::StockProductUnit(const QString &unit,
                                   qreal baseUnitEquivalent) :
    unit(unit),
    baseUnitEquivalent(baseUnitEquivalent)
{
}
