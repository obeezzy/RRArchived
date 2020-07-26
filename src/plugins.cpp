#include "plugins.h"
#include <QDirIterator>
#include <QFontDatabase>
#include <QtQml>

// QML API
// * Client
#include "rrcore/qmlapi/client/qmlclientmodel.h"
// * Creditor
#include "rrcore/qmlapi/creditor/qmlcreditormodel.h"
// * Dashboard
#include "rrcore/qmlapi/dashboard/qmldashboardhomemodel.h"
// * Debtor
#include "rrcore/qmlapi/debtor/qmldebtordetailrecord.h"
#include "rrcore/qmlapi/debtor/qmldebtormodel.h"
#include "rrcore/qmlapi/debtor/qmldebtpaymentmodel.h"
#include "rrcore/qmlapi/debtor/qmldebttransactionmodel.h"
// * Expense
#include "rrcore/qmlapi/expense/qmlexpensehomemodel.h"
#include "rrcore/qmlapi/expense/qmlexpensepusher.h"
#include "rrcore/qmlapi/expense/qmlexpensereportmodel.h"
#include "rrcore/qmlapi/expense/qmlexpensetransactionmodel.h"
// * Income
#include "rrcore/qmlapi/income/qmlincomehomemodel.h"
#include "rrcore/qmlapi/income/qmlincomepusher.h"
#include "rrcore/qmlapi/income/qmlincomereportmodel.h"
#include "rrcore/qmlapi/income/qmlincometransactionmodel.h"
// * KPI
#include "rrcore/qmlapi/kpi/qmlmostsoldproductmodel.h"
// * Purchase
#include "rrcore/qmlapi/purchase/qmlpurchasecartmodel.h"
#include "rrcore/qmlapi/purchase/qmlpurchasehomemodel.h"
#include "rrcore/qmlapi/purchase/qmlpurchasereportmodel.h"
#include "rrcore/qmlapi/purchase/qmlpurchasetransactionitemmodel.h"
#include "rrcore/qmlapi/purchase/qmlpurchasetransactionmodel.h"
// * Sales
#include "rrcore/qmlapi/sales/qmlsalecartmodel.h"
#include "rrcore/qmlapi/sales/qmlsalehomemodel.h"
#include "rrcore/qmlapi/sales/qmlsalereportmodel.h"
#include "rrcore/qmlapi/sales/qmlsaletransactionitemmodel.h"
#include "rrcore/qmlapi/sales/qmlsaletransactionmodel.h"
// * Stock
#include "rrcore/qmlapi/stock/qmlproductcategorymodel.h"
#include "rrcore/qmlapi/stock/qmlproductcountrecord.h"
#include "rrcore/qmlapi/stock/qmlproductdetailmodel.h"
#include "rrcore/qmlapi/stock/qmlproductmodel.h"
#include "rrcore/qmlapi/stock/qmlproductpusher.h"
#include "rrcore/qmlapi/stock/qmlstockreportmodel.h"
// * User
#include "rrcore/qmlapi/user/qmluserdetailrecord.h"
#include "rrcore/qmlapi/user/qmlusermodel.h"
#include "rrcore/qmlapi/user/qmluserprivilegemodel.h"
#include "rrcore/qmlapi/user/qmluserprofile.h"
// * Util
#include "rrcore/qmlapi/util/qmldatabasecreator.h"
#include "rrcore/qmlapi/util/qmldoublevalidator.h"
#include "rrcore/qmlapi/util/qmlnotifier.h"
#include "rrcore/qmlapi/util/qmlreceiptprinter.h"
#include "rrcore/qmlapi/util/qmlsettings.h"
// * Widgets
#include "rrcore/widgets/dialogs.h"

// User
#include "rrcore/user/businessdetails.h"
#include "rrcore/user/businessstore.h"
#include "rrcore/user/businessstoremodel.h"

const QString FONT_DIR(":/fonts");

Plugins::Plugins() {}

void Plugins::registerTypes()
{
    // Components
    qmlRegisterType<QMLDoubleValidator>("com.gecko.rr.components", 1, 0,
                                        "DoubleValidator");

    // QML API
    // * Client
    qmlRegisterType<QMLClientModel>("com.gecko.rr.models", 1, 0, "ClientModel");
    // * Creditor
    qmlRegisterType<QMLCreditorModel>("com.gecko.rr.models", 1, 0,
                                      "CreditorModel");
    // * Dashboard
    qmlRegisterType<QMLDashboardHomeModel>("com.gecko.rr.models", 1, 0,
                                           "DashboardHomeModel");
    // * Debtor
    qmlRegisterType<QMLDebtorDetailRecord>("com.gecko.rr.models", 1, 0,
                                           "DebtorDetailRecord");
    qmlRegisterType<QMLDebtorModel>("com.gecko.rr.models", 1, 0, "DebtorModel");
    qmlRegisterType<QMLDebtPaymentModel>("com.gecko.rr.models", 1, 0,
                                         "DebtPaymentModel");
    qmlRegisterType<QMLDebtTransactionModel>("com.gecko.rr.models", 1, 0,
                                             "DebtTransactionModel");
    // * Expense
    qmlRegisterType<QMLExpenseHomeModel>("com.gecko.rr.models", 1, 0,
                                         "ExpenseHomeModel");
    qmlRegisterType<QMLExpensePusher>("com.gecko.rr.models", 1, 0,
                                      "ExpensePusher");
    qmlRegisterType<QMLExpenseReportModel>("com.gecko.rr.models", 1, 0,
                                           "ExpenseReportModel");
    qmlRegisterType<QMLExpenseTransactionModel>("com.gecko.rr.models", 1, 0,
                                                "ExpenseTransactionModel");
    // * Income
    qmlRegisterType<QMLIncomeHomeModel>("com.gecko.rr.models", 1, 0,
                                        "IncomeHomeModel");
    qmlRegisterType<QMLIncomePusher>("com.gecko.rr.models", 1, 0,
                                     "IncomePusher");
    qmlRegisterType<QMLIncomeReportModel>("com.gecko.rr.models", 1, 0,
                                          "IncomeReportModel");
    qmlRegisterType<QMLIncomeTransactionModel>("com.gecko.rr.models", 1, 0,
                                               "IncomeTransactionModel");
    // * Purchase
    qmlRegisterType<QMLPurchaseCartModel>("com.gecko.rr.models", 1, 0,
                                          "PurchaseCartModel");
    qmlRegisterType<QMLPurchaseHomeModel>("com.gecko.rr.models", 1, 0,
                                          "PurchaseHomeModel");
    qmlRegisterType<QMLPurchaseReportModel>("com.gecko.rr.models", 1, 0,
                                            "PurchaseReportModel");
    qmlRegisterType<QMLPurchaseTransactionModel>("com.gecko.rr.models", 1, 0,
                                                 "PurchaseTransactionModel");
    qmlRegisterType<QMLPurchaseTransactionItemModel>(
        "com.gecko.rr.models", 1, 0, "PurchaseTransactionItemModel");
    // * Sales
    qmlRegisterType<QMLSaleCartModel>("com.gecko.rr.models", 1, 0,
                                      "SaleCartModel");
    qmlRegisterType<QMLSaleHomeModel>("com.gecko.rr.models", 1, 0,
                                      "SaleHomeModel");
    qmlRegisterType<QMLSaleReportModel>("com.gecko.rr.models", 1, 0,
                                        "SaleReportModel");
    qmlRegisterType<QMLSaleTransactionModel>("com.gecko.rr.models", 1, 0,
                                             "SaleTransactionModel");
    qmlRegisterType<QMLSaleTransactionItemModel>("com.gecko.rr.models", 1, 0,
                                                 "SaleTransactionItemModel");
    // * Stock
    qmlRegisterType<QMLProductCategoryModel>("com.gecko.rr.models", 1, 0,
                                             "ProductCategoryModel");
    qmlRegisterType<QMLProductCountRecord>("com.gecko.rr.models", 1, 0,
                                           "ProductCountRecord");
    qmlRegisterType<QMLProductDetailModel>("com.gecko.rr.models.stock", 1, 0,
                                           "ProductDetailModel");
    qmlRegisterType<QMLProductPusher>("com.gecko.rr.models", 1, 0,
                                      "ProductPusher");
    qmlRegisterType<QMLProductModel>("com.gecko.rr.models", 1, 0,
                                     "ProductModel");
    qmlRegisterType<QMLStockReportModel>("com.gecko.rr.models", 1, 0,
                                         "StockReportModel");
    // * User
    qmlRegisterType<QMLUserDetailRecord>("com.gecko.rr.models", 1, 0,
                                         "UserDetailRecord");
    qmlRegisterType<QMLUserModel>("com.gecko.rr.models", 1, 0, "UserModel");
    qmlRegisterType<QMLUserPrivilegeModel>("com.gecko.rr.models", 1, 0,
                                           "UserPrivilegeModel");
    qmlRegisterType<QMLUserProfile>("com.gecko.rr", 1, 0, "UserProfile");
    // * Util
    qmlRegisterType<QMLDatabaseCreator>("com.gecko.rr", 1, 0,
                                        "DatabaseCreator");
    qmlRegisterSingletonType<QMLNotifier>("com.gecko.rr", 1, 0, "Notifier",
                                          notifier_provider);
    qmlRegisterType<QMLReceiptPrinter>("com.gecko.rr", 1, 0, "ReceiptPrinter");
    qmlRegisterType<QMLSettings>("com.gecko.rr", 1, 0, "Settings");
    // * Widgets
    qmlRegisterSingletonType<Dialogs>("com.gecko.rr.widgets", 1, 0, "Dialogs",
                                      dialogs_provider);

    // User
    qmlRegisterUncreatableType<BusinessDetails>(
        "com.gecko.rr", 1, 0, "BusinessDetails", "Don't you dare create me!");
    qmlRegisterUncreatableType<BusinessStore>(
        "com.gecko.rr", 1, 0, "BusinessStore", "Don't you dare create me!");
    qmlRegisterUncreatableType<BusinessStoreModel>("com.gecko.rr", 1, 0,
                                                   "BusinessStoreModel",
                                                   "Don't you dare create me!");
}

void Plugins::registerFonts()
{
    QDirIterator iter(FONT_DIR);
    while (iter.hasNext()) {
        const QString& fontPath = iter.next();
        qInfo() << "Adding font..." << fontPath;
        QFontDatabase::addApplicationFont(fontPath);
    }
}
