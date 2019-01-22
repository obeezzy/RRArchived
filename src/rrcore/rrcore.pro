TEMPLATE = lib

QT += core qml quick quickcontrols2 sql svg printsupport

CONFIG += c++14

RESOURCES += sql/sql.qrc \
    config/config.qrc \
    json/json.qrc

INCLUDEPATH += .

SOURCES += \
    qmlapi/qmluserprofile.cpp \
    sqlmanager/usersqlmanager.cpp \
    sqlmanager/abstractsqlmanager.cpp \
    database/databasethread.cpp \
    database/databaseserver.cpp \
    database/databasecreator.cpp \
    backup/backupthread.cpp \
    backup/jsonlogger.cpp \
    database/databaseexception.cpp \
    database/queryrequest.cpp \
    database/queryresult.cpp \
    qmlapi/qmldashboardhomemodel.cpp \
    sqlmanager/dashboardsqlmanager.cpp \
    qmlapi/qmlstockitempusher.cpp \
    sqlmanager/stocksqlmanager.cpp \
    singletons/userprofile.cpp \
    database/databaseutils.cpp \
    models/stockitemmodel.cpp \
    models/abstractvisuallistmodel.cpp \
    pusher/abstractpusher.cpp \
    qmlapi/qmlsalecartmodel.cpp \
    sqlmanager/salesqlmanager.cpp \
    qmlapi/qmlsaletransactionmodel.cpp \
    qmlapi/qmldoublevalidator.cpp \
    qmlapi/qmlsaletransactionitemmodel.cpp \
    qmlapi/qmlstockcategoryitemmodel.cpp \
    qmlapi/qmlstockcategorymodel.cpp \
    qmlapi/qmldebtormodel.cpp \
    sqlmanager/debtorsqlmanager.cpp \
    qmlapi/qmlsalehomemodel.cpp \
    models/saletotalrevenuemodel.cpp \
    models/salemostsolditemmodel.cpp \
    detailrecord/abstractdetailrecord.cpp \
    qmlapi/qmlstockitemdetailrecord.cpp \
    models/abstracttransactionmodel.cpp \
    models/abstracttransactionitemmodel.cpp \
    widgets/dialogs.cpp \
    qmlapi/qmlclientmodel.cpp \
    sqlmanager/clientsqlmanager.cpp \
    qmlapi/qmldebttransactionmodel.cpp \
    models/debtpaymentmodel.cpp \
    qmlapi/qmldebtordetailrecord.cpp \
    models/salepaymentmodel.cpp \
    qmlapi/qmldatabasecreator.cpp \
    config/config.cpp \
    json/messagecenter.cpp \
    models/abstracthomemodel.cpp \
    qmlapi/qmlnotifier.cpp \
    qmlapi/qmlpurchasehomemodel.cpp \
    sqlmanager/purchasesqlmanager.cpp \
    qmlapi/qmlpurchasecartmodel.cpp \
    models/purchasepaymentmodel.cpp \
    qmlapi/qmlincomehomemodel.cpp \
    qmlapi/qmlincomepusher.cpp \
    qmlapi/qmlexpensepusher.cpp \
    qmlapi/qmlexpensehomemodel.cpp \
    qmlapi/qmlcreditormodel.cpp \
    sqlmanager/incomesqlmanager.cpp \
    sqlmanager/expensesqlmanager.cpp

HEADERS += \
    qmlapi/qmluserprofile.h \
    sqlmanager/usersqlmanager.h \
    sqlmanager/abstractsqlmanager.h \
    database/databasethread.h \
    database/databaseserver.h \
    database/databasecreator.h \
    backup/backupthread.h \
    backup/jsonlogger.h \
    database/databaseexception.h \
    database/queryrequest.h \
    database/queryresult.h \
    qmlapi/qmldashboardhomemodel.h \
    sqlmanager/dashboardsqlmanager.h \
    qmlapi/qmlstockitempusher.h \
    sqlmanager/stocksqlmanager.h \
    singletons/userprofile.h \
    database/databaseutils.h \
    models/stockitemmodel.h \
    models/abstractvisuallistmodel.h \
    pusher/abstractpusher.h \
    qmlapi/qmlsalecartmodel.h \
    sqlmanager/salesqlmanager.h \
    qmlapi/qmlsaletransactionmodel.h \
    qmlapi/qmldoublevalidator.h \
    qmlapi/qmlsaletransactionitemmodel.h \
    qmlapi/qmlstockcategoryitemmodel.h \
    qmlapi/qmlstockcategorymodel.h \
    qmlapi/qmldebtormodel.h \
    sqlmanager/debtorsqlmanager.h \
    qmlapi/qmlsalehomemodel.h \
    models/saletotalrevenuemodel.h \
    models/salemostsolditemmodel.h \
    detailrecord/abstractdetailrecord.h \
    qmlapi/qmlstockitemdetailrecord.h \
    models/abstracttransactionmodel.h \
    models/abstracttransactionitemmodel.h \
    widgets/dialogs.h \
    qmlapi/qmlclientmodel.h \
    sqlmanager/clientsqlmanager.h \
    qmlapi/qmldebttransactionmodel.h \
    models/debtpaymentmodel.h \
    qmlapi/qmldebtordetailrecord.h \
    utility/debtorutils.h \
    utility/saleutils.h \
    models/salepaymentmodel.h \
    qmlapi/qmldatabasecreator.h \
    config/config.h \
    json/messagecenter.h \
    models/abstracthomemodel.h \
    qmlapi/qmlnotifier.h \
    qmlapi/qmlpurchasehomemodel.h \
    sqlmanager/purchasesqlmanager.h \
    qmlapi/qmlpurchasecartmodel.h \
    models/purchasepaymentmodel.h \
    qmlapi/qmlincomehomemodel.h \
    qmlapi/qmlincomepusher.h \
    qmlapi/qmlexpensepusher.h \
    qmlapi/qmlexpensehomemodel.h \
    qmlapi/qmlcreditormodel.h \
    sqlmanager/incomesqlmanager.h \
    sqlmanager/expensesqlmanager.h
