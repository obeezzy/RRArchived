#include "plugins.h"
#include <QtQml>

#include "rr-core/qml_api/qmluserprofile.h"
#include "rr-core/qml_api/qmldashboardhomemodel.h"
#include "rr-core/qml_api/qmlstockcategoryitemmodel.h"
#include "rr-core/qml_api/qmlstockitempusher.h"
#include "rr-core/qml_api/qmlsalecartmodel.h"
#include "rr-core/qml_api/qmlsaletransactionmodel.h"
#include "rr-core/qml_api/qmlsaletransactionitemmodel.h"
#include "rr-core/qml_api/qmldoublevalidator.h"
#include "rr-core/qml_api/qmlstockcategorymodel.h"

Plugins::Plugins()
{

}

void Plugins::registerTypes()
{
    // Misc
    qmlRegisterType<QMLUserProfile>("com.gecko.rr", 1, 0, "UserProfile");

    // Models
    qmlRegisterType<QMLDashboardHomeModel>("com.gecko.rr.models", 1, 0, "DashboardHomeModel");
    qmlRegisterType<QMLStockCategoryItemModel>("com.gecko.rr.models", 1, 0, "StockCategoryItemModel");
    qmlRegisterType<QMLStockItemPusher>("com.gecko.rr.models", 1, 0, "StockItemPusher");
    qmlRegisterType<QMLSaleCartModel>("com.gecko.rr.models", 1, 0, "SaleCartModel");
    qmlRegisterType<QMLSaleTransactionModel>("com.gecko.rr.models", 1, 0, "SaleTransactionModel");
    qmlRegisterType<QMLSaleTransactionItemModel>("com.gecko.rr.models", 1, 0, "SaleTransactionItemModel");
    qmlRegisterType<QMLStockCategoryModel>("com.gecko.rr.models", 1, 0, "StockCategoryModel");

    // Components
    qmlRegisterType<QMLDoubleValidator>("com.gecko.rr.components", 1, 0, "DoubleValidator");
}
