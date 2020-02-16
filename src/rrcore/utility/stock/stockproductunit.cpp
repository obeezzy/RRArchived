#include "stockproductunit.h"

using namespace Utility;

StockProductUnit::StockProductUnit(int id,
                                   const QString &unit) :
    id(id),
    unit(unit)
{}

StockProductUnit::StockProductUnit(const QString &unit,
                                   qreal baseUnitEquivalent) :
    unit(unit),
    baseUnitEquivalent(baseUnitEquivalent)
{}
