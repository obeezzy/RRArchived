#include "stockproductunit.h"

using namespace Utility;

StockProductUnit::StockProductUnit(int id,
                                   const QString &unit) :
    id(id),
    unit(unit)
{}

StockProductUnit::StockProductUnit(const QString &unit,
                                   double baseUnitEquivalent) :
    unit(unit),
    baseUnitEquivalent(baseUnitEquivalent)
{}
