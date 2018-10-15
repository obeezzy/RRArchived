#include "plugins.h"
#include <QtQml>

#include "rrcore/qmlapi/qmluserprofile.h"
#include "rrcore/qmlapi/qmldatabasecreator.h"

#include "rrcore/qmlapi/qmldashboardhomemodel.h"
#include "rrcore/qmlapi/qmlstockcategoryitemmodel.h"
#include "rrcore/qmlapi/qmlstockitempusher.h"
#include "rrcore/qmlapi/qmlsalecartmodel.h"
#include "rrcore/qmlapi/qmlsaletransactionmodel.h"
#include "rrcore/qmlapi/qmlsaletransactionitemmodel.h"
#include "rrcore/qmlapi/qmldoublevalidator.h"
#include "rrcore/qmlapi/qmlstockcategorymodel.h"
#include "rrcore/qmlapi/qmldebtormodel.h"
#include "rrcore/qmlapi/qmlsalehomemodel.h"
#include "rrcore/qmlapi/qmlstockitemdetailrecord.h"
#include "rrcore/qmlapi/qmlclientmodel.h"
#include "rrcore/qmlapi/qmldebttransactionmodel.h"
#include "rrcore/qmlapi/qmldebtordetailrecord.h"

#include "rrcore/widgets/dialogs.h"

Plugins::Plugins()
{

}

void Plugins::registerTypes()
{
    // Misc
    qmlRegisterType<QMLUserProfile>("com.gecko.rr", 1, 0, "UserProfile");
    qmlRegisterType<QMLDatabaseCreator>("com.gecko.rr", 1, 0, "DatabaseCreator");

    // Models
    qmlRegisterType<QMLDashboardHomeModel>("com.gecko.rr.models", 1, 0, "DashboardHomeModel");
    qmlRegisterType<QMLStockCategoryItemModel>("com.gecko.rr.models", 1, 0, "StockCategoryItemModel");
    qmlRegisterType<QMLStockItemPusher>("com.gecko.rr.models", 1, 0, "StockItemPusher");
    qmlRegisterType<QMLSaleCartModel>("com.gecko.rr.models", 1, 0, "SaleCartModel");
    qmlRegisterType<QMLSaleTransactionModel>("com.gecko.rr.models", 1, 0, "SaleTransactionModel");
    qmlRegisterType<QMLSaleTransactionItemModel>("com.gecko.rr.models", 1, 0, "SaleTransactionItemModel");
    qmlRegisterType<QMLStockCategoryModel>("com.gecko.rr.models", 1, 0, "StockCategoryModel");
    qmlRegisterType<QMLDebtorModel>("com.gecko.rr.models", 1, 0, "DebtorModel");
    qmlRegisterType<QMLSaleHomeModel>("com.gecko.rr.models", 1, 0, "SaleHomeModel");
    qmlRegisterType<QMLStockItemDetailRecord>("com.gecko.rr.models", 1, 0, "StockItemDetailRecord");
    qmlRegisterType<QMLClientModel>("com.gecko.rr.models", 1, 0, "ClientModel");
    qmlRegisterType<QMLDebtTransactionModel>("com.gecko.rr.models", 1, 0, "DebtTransactionModel");
    qmlRegisterType<QMLDebtorDetailRecord>("com.gecko.rr.models", 1, 0, "DebtorDetailRecord");

    // Components
    qmlRegisterType<QMLDoubleValidator>("com.gecko.rr.components", 1, 0, "DoubleValidator");

    // Widgets
    qmlRegisterSingletonType<Dialogs>("com.gecko.rr.widgets", 1, 0, "Dialogs", dialogs_provider);
}
