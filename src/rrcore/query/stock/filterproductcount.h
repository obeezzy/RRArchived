#ifndef FILTERPRODUCTCOUNT_H
#define FILTERPRODUCTCOUNT_H

#include "stockexecutor.h"

namespace Utility {
class FilterCriteria;
}

namespace Query {
    namespace Stock {
    class FilterProductCount : public StockExecutor
    {
        Q_OBJECT
    public:
        static inline const QString COMMAND = QStringLiteral("filter_product_count");

        explicit FilterProductCount(const Utility::FilterCriteria &filterCriteria,
                                         int categoryId,
                                         QObject *receiver);
        QueryResult execute() override;
    };
}
}

#endif // FILTERPRODUCTCOUNT_H
