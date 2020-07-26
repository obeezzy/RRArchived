#ifndef VIEWSALETRANSACTIONS_H
#define VIEWSALETRANSACTIONS_H

#include "saleexecutor.h"
#include "utility/common/recordgroup.h"

namespace Utility {
class DateTimeSpan;
}

namespace Query {
    namespace Sales {
    class ViewSaleTransactions : public SaleExecutor
    {
        Q_OBJECT
    public:
        static inline const QString COMMAND = QStringLiteral("view_sale_transactions");

        explicit ViewSaleTransactions(const Utility::DateTimeSpan &dateTimeSpan,
                                      const Utility::RecordGroup::Flags &flags,
                                      QObject *receiver);
        QueryResult execute() override;
    };
}
}

#endif // VIEWSALETRANSACTIONS_H
