import qbs
import qbs.Environment

Project {
    name: "tests"
    condition: project.withTests

    AutotestRunner { }

    references: [
        "QMLClientModel/QMLClientModel.qbs",
        "QMLCreditorModel/QMLCreditorModel.qbs",
        "QMLDashboardHomeModel/QMLDashboardHomeModel.qbs",
        "QMLDebtorDetailRecord/QMLDebtorDetailRecord.qbs",
        "QMLDebtorModel/QMLDebtorModel.qbs",
        "QMLDebtTransactionModel/QMLDebtTransactionModel.qbs",
        "QMLExpenseHomeModel/QMLExpenseHomeModel.qbs",
        "QMLExpensePusher/QMLExpensePusher.qbs",
        "QMLExpenseReportModel/QMLExpenseReportModel.qbs",
        "QMLIncomeHomeModel/QMLIncomeHomeModel.qbs",
        "QMLIncomePusher/QMLIncomePusher.qbs",
        "QMLIncomeReportModel/QMLIncomeReportModel.qbs",
        "QMLPurchaseCartModel/QMLPurchaseCartModel.qbs",
        "QMLPurchaseHomeModel/QMLPurchaseHomeModel.qbs",
        "QMLPurchaseReportModel/QMLPurchaseReportModel.qbs",
        "QMLPurchaseTransactionItemModel/QMLPurchaseTransactionItemModel.qbs",
        "QMLPurchaseTransactionModel/QMLPurchaseTransactionModel.qbs",
        "QMLSaleCartModel/QMLSaleCartModel.qbs",
        "QMLSaleHomeModel/QMLSaleHomeModel.qbs",
        "QMLSaleReportModel/QMLSaleReportModel.qbs",
        "QMLSaleTransactionItemModel/QMLSaleTransactionItemModel.qbs",
        "QMLSaleTransactionModel/QMLSaleTransactionModel.qbs",
        "QMLStockCategoryItemModel/QMLStockCategoryItemModel.qbs",
        "QMLStockCategoryModel/QMLStockCategoryModel.qbs",
        "QMLStockItemDetailRecord/QMLStockItemDetailRecord.qbs",
        "QMLStockItemPusher/QMLStockItemPusher.qbs",
        "QMLStockReportModel/QMLStockReportModel.qbs",
        "QMLUserDetailRecord/QMLUserDetailRecord.qbs",
        "QMLUserModel/QMLUserModel.qbs",
        "QMLUserPrivilegeModel/QMLUserPrivilegeModel.qbs",
        "QMLUserProfile/QMLUserProfile.qbs"
    ]
}
