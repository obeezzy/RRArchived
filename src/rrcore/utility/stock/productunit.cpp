#include "productunit.h"

using namespace Utility::Stock;

ProductUnit::ProductUnit(const QVariantMap& map)
    : id(map.value("product_unit_id").toInt()),
      productId(map.value("product_id").toInt()),
      unit(map.value("product_unit").toString()),
      baseUnitEquivalent(map.value("base_unit_equivalent").toDouble()),
      flags(map.value("preferred").toBool() ? Utility::RecordGroup::Preferred
                                            : Utility::RecordGroup::None)
{}

ProductUnit::ProductUnit(int id, const QString& unit) : id(id), unit(unit) {}

ProductUnit::ProductUnit(const QString& unit, double baseUnitEquivalent)
    : unit(unit),
      baseUnitEquivalent(baseUnitEquivalent)
{}
