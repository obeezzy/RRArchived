#ifndef VIEWDASHBOARD_H
#define VIEWDASHBOARD_H

#include "dashboardexecutor.h"

namespace Query {
    namespace Dashboard {
    class ViewDashboard : public DashboardExecutor
    {
        Q_OBJECT
    public:
        static inline const QString COMMAND = QStringLiteral("view_dashboard");

        explicit ViewDashboard(QObject *receiver);
        QueryResult execute() override;
    private:
        void fetchWelcomeMessage(QVariantList &homeRecords);
        void fetchEmptyStockMessage(QVariantList &homeRecords);
    };
}
}

#endif // VIEWDASHBOARD_H
