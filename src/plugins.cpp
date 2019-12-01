#include "plugins.h"
#include <QtQml>
#include <QFontDatabase>
#include <QDirIterator>

#include "rrcore/qmlapi/qmluserprofile.h"
#include "rrcore/qmlapi/qmldatabasecreator.h"

#include "rrcore/qmlapi/qmldashboardhomemodel.h"
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
#include "rrcore/qmlapi/qmlnotifier.h"
#include "rrcore/qmlapi/qmlpurchasehomemodel.h"
#include "rrcore/qmlapi/qmlpurchasecartmodel.h"
#include "rrcore/qmlapi/qmlincomehomemodel.h"
#include "rrcore/qmlapi/qmlincomepusher.h"
#include "rrcore/qmlapi/qmlexpensehomemodel.h"
#include "rrcore/qmlapi/qmlexpensepusher.h"
#include "rrcore/qmlapi/qmlcreditormodel.h"
#include "rrcore/qmlapi/qmlincometransactionmodel.h"
#include "rrcore/qmlapi/qmlusermodel.h"
#include "rrcore/qmlapi/qmluserprivilegemodel.h"
#include "rrcore/qmlapi/qmluserdetailrecord.h"
#include "rrcore/qmlapi/qmlsettings.h"
#include "rrcore/qmlapi/qmlpurchasetransactionmodel.h"
#include "rrcore/qmlapi/qmlpurchasetransactionitemmodel.h"
#include "rrcore/qmlapi/qmlreceiptprinter.h"
#include "rrcore/qmlapi/qmlstockreportmodel.h"
#include "rrcore/qmlapi/qmlsalereportmodel.h"
#include "rrcore/qmlapi/qmlpurchasereportmodel.h"
#include "rrcore/qmlapi/qmlincomereportmodel.h"
#include "rrcore/qmlapi/qmlexpensereportmodel.h"
#include "rrcore/qmlapi/qmlexpensetransactionmodel.h"
#include "rrcore/qmlapi/qmlstockitemmodel.h"
#include "rrcore/qmlapi/qmlstockitemcountrecord.h"

#include "rrcore/widgets/dialogs.h"
#include "rrcore/user/businessdetails.h"
#include "rrcore/user/businessstore.h"
#include "rrcore/user/businessstoremodel.h"

const QString FONT_DIR(":/fonts");

Plugins::Plugins()
{

}

void Plugins::registerTypes()
{
    // Misc
    qmlRegisterType<QMLUserProfile>("com.gecko.rr", 1, 0, "UserProfile");
    qmlRegisterType<QMLDatabaseCreator>("com.gecko.rr", 1, 0, "DatabaseCreator");
    qmlRegisterSingletonType<QMLNotifier>("com.gecko.rr", 1, 0, "Notifier", notifier_provider);
    qmlRegisterType<QMLSettings>("com.gecko.rr", 1, 0, "Settings");
    qmlRegisterType<QMLReceiptPrinter>("com.gecko.rr", 1, 0, "ReceiptPrinter");

    qmlRegisterUncreatableType<BusinessDetails>("com.gecko.rr", 1, 0, "BusinessDetails", "Don't you dare create me!");
    qmlRegisterUncreatableType<BusinessStore>("com.gecko.rr", 1, 0, "BusinessStore", "Don't you dare create me!");
    qmlRegisterUncreatableType<BusinessStoreModel>("com.gecko.rr", 1, 0, "BusinessStoreModel", "Don't you dare create me!");

    // Models
    qmlRegisterType<QMLDashboardHomeModel>("com.gecko.rr.models", 1, 0, "DashboardHomeModel");
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
    qmlRegisterType<QMLPurchaseHomeModel>("com.gecko.rr.models", 1, 0, "PurchaseHomeModel");
    qmlRegisterType<QMLPurchaseCartModel>("com.gecko.rr.models", 1, 0, "PurchaseCartModel");
    qmlRegisterType<QMLIncomeHomeModel>("com.gecko.rr.models", 1, 0, "IncomeHomeModel");
    qmlRegisterType<QMLIncomePusher>("com.gecko.rr.models", 1, 0, "IncomePusher");
    qmlRegisterType<QMLExpenseHomeModel>("com.gecko.rr.models", 1, 0, "ExpenseHomeModel");
    qmlRegisterType<QMLExpensePusher>("com.gecko.rr.models", 1, 0, "ExpensePusher");
    qmlRegisterType<QMLCreditorModel>("com.gecko.rr.models", 1, 0, "CreditorModel");
    qmlRegisterType<QMLIncomeTransactionModel>("com.gecko.rr.models", 1, 0, "IncomeTransactionModel");
    qmlRegisterType<QMLUserModel>("com.gecko.rr.models", 1, 0, "UserModel");
    qmlRegisterType<QMLUserPrivilegeModel>("com.gecko.rr.models", 1, 0, "UserPrivilegeModel");
    qmlRegisterType<QMLUserDetailRecord>("com.gecko.rr.models", 1, 0, "UserDetailRecord");
    qmlRegisterType<QMLPurchaseTransactionModel>("com.gecko.rr.models", 1, 0, "PurchaseTransactionModel");
    qmlRegisterType<QMLPurchaseTransactionItemModel>("com.gecko.rr.models", 1, 0, "PurchaseTransactionItemModel");
    qmlRegisterType<QMLStockReportModel>("com.gecko.rr.models", 1, 0, "StockReportModel");
    qmlRegisterType<QMLSaleReportModel>("com.gecko.rr.models", 1, 0, "SaleReportModel");
    qmlRegisterType<QMLPurchaseReportModel>("com.gecko.rr.models", 1, 0, "PurchaseReportModel");
    qmlRegisterType<QMLIncomeReportModel>("com.gecko.rr.models", 1, 0, "IncomeReportModel");
    qmlRegisterType<QMLExpenseReportModel>("com.gecko.rr.models", 1, 0, "ExpenseReportModel");
    qmlRegisterType<QMLExpenseTransactionModel>("com.gecko.rr.models", 1, 0, "ExpenseTransactionModel");
    qmlRegisterType<QMLStockItemModel>("com.gecko.rr.models", 1, 0, "StockItemModel");
    qmlRegisterType<QMLStockItemCountRecord>("com.gecko.rr.models", 1, 0, "StockItemCountRecord");

    // Components
    qmlRegisterType<QMLDoubleValidator>("com.gecko.rr.components", 1, 0, "DoubleValidator");

    // Widgets
    qmlRegisterSingletonType<Dialogs>("com.gecko.rr.widgets", 1, 0, "Dialogs", dialogs_provider);
}

void Plugins::registerFonts()
{
    QDirIterator iter(FONT_DIR);
    while (iter.hasNext()) {
        const QString &fontPath = iter.next();
        qInfo() << "Adding font..." << fontPath;
        QFontDatabase::addApplicationFont(fontPath);
    }
}
