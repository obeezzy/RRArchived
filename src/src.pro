TEMPLATE = app
TARGET = rr

QT += qml quick quickcontrols2 sql charts

CONFIG += c++14

INCLUDEPATH += rrcore
LIBS += -L$$OUT_PWD/rrcore -lrrcore

SOURCES += main.cpp \
    plugins.cpp

HEADERS += plugins.h

#RESOURCES += qml.qrc

DISTFILES += \
    qml/main.qml \
    qml/user/LoginPage.qml \
    qml/rrui/TextButton.qml \
    qml/common/DashboardPage.qml \
    qml/rrui/Sidebar.qml \
    qml/models/NavMenuModel.qml \
    qml/stock/NewItemPage.qml \
    qml/stock/HomePage.qml \
    qml/sales/HomePage.qml \
    qml/rrui/FailureAlertDialogLoader.qml \
    qml/rrui/BusyOverlay.qml \
    qml/sales/NewSalePage.qml \
    qml/common/CategoryListView.qml \
    qml/rrui/QuantitySpinBox.qml \
    qml/common/paymentwizard/PaymentMethodPage.qml \
    qml/common/paymentwizard/PaymentByCardPage.qml \
    qml/common/paymentwizard/PaymentBalancePage.qml \
    qml/common/PaymentWizard.qml \
    qml/common/paymentwizard/PaymentFinishPage.qml \
    qml/common/paymentwizard/PaymentByCashPage.qml \
    qml/common/paymentwizard/PaymentDebtorPage.qml \
    qml/common/paymentwizard/PaymentCreditorPage.qml \
    qml/common/paymentwizard/PaymentCustomerDetailPage.qml \
    qml/common/paymentwizard/PaymentDueDatePage.qml \
    qml/common/paymentwizard/PaymentSummaryPage.qml \
    qml/rrui/Chip.qml \
    qml/rrui/ChipListView.qml \
    qml/common/SaleCartListView.qml \
    qml/rrui/RadioButtonDialog.qml \
    qml/rrui/SearchBar.qml \
    qml/singletons/Stylesheet.qml \
    qml/singletons/GlobalSettings.qml \
    qml/common/paymentwizard/PaymentChangePage.qml \
    qml/stock/SectionFragment.qml \
    qml/common/SuspendTransactionDialog.qml \
    qml/common/RetrieveTransactionDialog.qml \
    qml/people/debtor/HomePage.qml \
    qml/people/debtor/NewDebtorPage.qml \
    qml/common/DebtorListView.qml \
    qml/sales/SaleHomeListView.qml \
    qml/sales/homecards/TotalRevenueCard.qml \
    qml/rrui/HomeCard.qml \
    qml/sales/homecards/MostSoldItemCard.qml \
    qml/rrui/TableView.qml \
    qml/rrui/SnackBar.qml
