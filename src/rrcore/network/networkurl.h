#ifndef NETWORKURL_H
#define NETWORKURL_H

#include <QString>

namespace NetworkUrl {
    inline static const QString SERVER_URL = QStringLiteral("http://localhost:3000");

    inline namespace Auth {
        inline static const QString SIGN_IN_API_URL = SERVER_URL + QStringLiteral("/api/auth/sign-in");
        inline static const QString SIGN_UP_API_URL = SERVER_URL + QStringLiteral("/api/auth/sign-up");
        inline static const QString SIGN_OUT_API_URL = SERVER_URL + QStringLiteral("/api/auth/sign-out");
        inline static const QString LINK_ACCOUNT_URL = SERVER_URL + QStringLiteral("/api/auth/link-account");
        inline static const QString LINK_BUSINESS_STORE_URL = SERVER_URL + QStringLiteral("/api/auth/link-business-store");
        inline static const QString CHANGE_PASSWORD_API_URL = SERVER_URL + QStringLiteral("/api/auth/change-password");
    }

    inline namespace Database {
        inline static const QString DASHBOARD_API_URL = SERVER_URL + QStringLiteral("/api/database/dashboard");
        inline static const QString STOCK_API_URL = SERVER_URL + QStringLiteral("/api/database/stock");
        inline static const QString SALES_API_URL = SERVER_URL + QStringLiteral("/api/database/sales");
        inline static const QString PURCHASE_API_URL = SERVER_URL + QStringLiteral("/api/database/purchase");
        inline static const QString INCOME_API_URL = SERVER_URL + QStringLiteral("/api/database/income");
        inline static const QString EXPENSE_API_URL = SERVER_URL + QStringLiteral("/api/database/expense");
        inline static const QString DEBTOR_API_URL = SERVER_URL + QStringLiteral("/api/database/debtor");
        inline static const QString CREDITOR_API_URL = SERVER_URL + QStringLiteral("/api/database/creditor");
        inline static const QString USER_API_URL = SERVER_URL + QStringLiteral("/api/database/user");
    }

    inline namespace View {
        inline static const QString NEW_USER_URL = SERVER_URL + QStringLiteral("/new-user");
        inline static const QString PASSWORD_RETRIEVAL_URL = SERVER_URL + QStringLiteral("/password-recovery");
    }
}
#endif // NETWORKURL_H
