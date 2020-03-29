#include "stockproductunit.h"

using namespace Utility;

StockProductUnit::StockProductUnit(const QVariantMap &map) :
    id(map.value("product_unit_id").toInt()),
    productId(map.value("product_id").toInt()),
    unit(map.value("product_unit").toString()),
    baseUnitEquivalent(map.value("base_unit_equivalent").toDouble()),
    flags(map.value("preferred").toBool() ? RecordGroup::Preferred : RecordGroup::None)
{}

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
