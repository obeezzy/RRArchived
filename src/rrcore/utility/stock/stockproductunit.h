#ifndef STOCKPRODUCTUNIT_H
#define STOCKPRODUCTUNIT_H

#include "utility/common/recordgroup.h"
#include <QString>

namespace Utility {
struct StockProductUnit
{
    int id {-1};
    QString unit;
    RecordGroup::Flags flags;
    qreal baseUnitEquivalent {1.0};

    explicit StockProductUnit() = default;
    explicit StockProductUnit(int id,
                              const QString &unit);
    explicit StockProductUnit(const QString &unit,
                              qreal baseUnitEquivalent);
};
}

#endif // STOCKPRODUCTUNIT_H
