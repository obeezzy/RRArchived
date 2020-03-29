#ifndef STOCKPRODUCTUNIT_H
#define STOCKPRODUCTUNIT_H

#include "utility/common/recordgroup.h"
#include <QDebug>
#include <QString>

namespace Utility {
struct StockProductUnit
{
    int id {0};
    int productId {0};
    QString unit;
    double baseUnitEquivalent {1.0};
    RecordGroup::Flags flags;

    explicit StockProductUnit() = default;
    explicit StockProductUnit(const QVariantMap &map);
    explicit StockProductUnit(int id,
                              const QString &unit);
    explicit StockProductUnit(const QString &unit,
                              double baseUnitEquivalent);

    friend QDebug operator<<(QDebug debug, const StockProductUnit &unit)
    {
        debug.nospace() << "StockProductUnit("
                        << "id=" << unit.id
                        << ", productId=" << unit.productId
                        << ", unit=" << unit.unit
                        << ", baseUnitEquivalent=" << unit.baseUnitEquivalent
                        << ")";

        return debug.nospace();
    }
};
}
Q_DECLARE_TYPEINFO(Utility::StockProductUnit, Q_PRIMITIVE_TYPE);

#endif // STOCKPRODUCTUNIT_H
