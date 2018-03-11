QT += qml quick quickcontrols2 sql

CONFIG += c++14

SOURCES += main.cpp \
    rr-core/qml_api/qmluserprofile.cpp \
    rr-core/sql_manager/usersqlmanager.cpp \
    rr-core/sql_manager/abstractsqlmanager.cpp \
    plugins.cpp \
    rr-core/database/databasethread.cpp \
    rr-core/database/databaseserver.cpp \
    rr-core/database/databasecreator.cpp \
    rr-core/backup/backupthread.cpp \
    rr-core/backup/jsonlogger.cpp \
    rr-core/qml_api/qmlcommandmodel.cpp \
    rr-core/database/databaseexception.cpp \
    rr-core/database/queryrequest.cpp \
    rr-core/database/queryresult.cpp \
    rr-core/qml_api/qmldashboardhomemodel.cpp \
    rr-core/sql_manager/dashboardsqlmanager.cpp \
    rr-core/qml_api/qmlstockitempusher.cpp \
    rr-core/sql_manager/stocksqlmanager.cpp \
    rr-core/singletons/userprofile.cpp \
    rr-core/database/databaseutils.cpp \
    rr-core/models/stockitemmodel.cpp \
    rr-core/models/abstractvisuallistmodel.cpp \
    rr-core/pusher/abstractpusher.cpp \
    rr-core/qml_api/qmlsalecartmodel.cpp \
    rr-core/sql_manager/salesqlmanager.cpp \
    rr-core/qml_api/qmlsaletransactionmodel.cpp \
    rr-core/qml_api/qmldoublevalidator.cpp \
    rr-core/qml_api/qmlsaletransactionitemmodel.cpp \
    rr-core/qml_api/qmlstockcategoryitemmodel.cpp \
    rr-core/qml_api/qmlstockcategorymodel.cpp

HEADERS += \
    rr-core/qml_api/qmluserprofile.h \
    rr-core/sql_manager/usersqlmanager.h \
    rr-core/sql_manager/abstractsqlmanager.h \
    plugins.h \
    rr-core/database/databasethread.h \
    rr-core/database/databaseserver.h \
    rr-core/database/databasecreator.h \
    rr-core/backup/backupthread.h \
    rr-core/backup/jsonlogger.h \
    rr-core/qml_api/qmlcommandmodel.h \
    rr-core/database/databaseexception.h \
    rr-core/database/queryrequest.h \
    rr-core/database/queryresult.h \
    rr-core/qml_api/qmldashboardhomemodel.h \
    rr-core/sql_manager/dashboardsqlmanager.h \
    rr-core/qml_api/qmlstockitempusher.h \
    rr-core/sql_manager/stocksqlmanager.h \
    rr-core/singletons/userprofile.h \
    rr-core/database/databaseutils.h \
    rr-core/models/stockitemmodel.h \
    rr-core/models/abstractvisuallistmodel.h \
    rr-core/pusher/abstractpusher.h \
    rr-core/qml_api/qmlsalecartmodel.h \
    rr-core/sql_manager/salesqlmanager.h \
    rr-core/qml_api/qmlsaletransactionmodel.h \
    rr-core/qml_api/qmldoublevalidator.h \
    rr-core/qml_api/qmlsaletransactionitemmodel.h \
    rr-core/qml_api/qmlstockcategoryitemmodel.h \
    rr-core/qml_api/qmlstockcategorymodel.h

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
    qml/common/RetrieveTransactionDialog.qml

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
