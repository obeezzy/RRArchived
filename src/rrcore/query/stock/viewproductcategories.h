#ifndef VIEWPRODUCTCATEGORIES_H
#define VIEWPRODUCTCATEGORIES_H

#include "stockexecutor.h"
#include "utility/common/recordgroup.h"

namespace Utility {
class SortCriteria;
}

namespace Query {
    namespace Stock {
    class ViewProductCategories : public StockExecutor
    {
    public:
        static inline const QString COMMAND = QStringLiteral("view_product_categories");

        explicit ViewProductCategories(const Utility::SortCriteria &sortCriteria,
                                        const Utility::RecordGroup::Flags &flags,
                                        QObject *receiver);
        QueryResult execute() override;
    };
}
}

#endif // VIEWPRODUCTCATEGORIES_H
