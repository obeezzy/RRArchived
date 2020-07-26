#ifndef STOCKPRODUCTUNIT_H
#define STOCKPRODUCTUNIT_H

#include "utility/common/recordgroup.h"
#include <QDebug>
#include <QString>

namespace Utility {
    namespace Stock {
    struct ProductUnit
    {
        int id {0};
        int productId {0};
        QString unit;
        double baseUnitEquivalent {1.0};
        RecordGroup::Flags flags;

        explicit ProductUnit() = default;
        explicit ProductUnit(const QVariantMap &map);
        explicit ProductUnit(int id,
                                  const QString &unit);
        explicit ProductUnit(const QString &unit,
                                  double baseUnitEquivalent);

        friend QDebug operator<<(QDebug debug, const ProductUnit &unit)
        {
            debug.nospace() << "ProductUnit("
                            << "id=" << unit.id
                            << ", productId=" << unit.productId
                            << ", unit=" << unit.unit
                            << ", baseUnitEquivalent=" << unit.baseUnitEquivalent
                            << ")";

            return debug.nospace();
        }
    };
}
}
Q_DECLARE_TYPEINFO(Utility::Stock::ProductUnit, Q_PRIMITIVE_TYPE);

#endif // STOCKPRODUCTUNIT_H
