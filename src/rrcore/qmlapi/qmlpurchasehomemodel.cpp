#include "qmlpurchasehomemodel.h"
#include "database/databasethread.h"
#include "queryexecutors/purchase.h"

QMLPurchaseHomeModel::QMLPurchaseHomeModel(QObject *parent) :
    QMLPurchaseHomeModel(DatabaseThread::instance(), parent)
{}

QMLPurchaseHomeModel::QMLPurchaseHomeModel(DatabaseThread &thread, QObject *parent) :
    AbstractHomeModel(thread, parent)
{}

void QMLPurchaseHomeModel::tryQuery()
{
    setBusy(true);
    emit execute(new PurchaseQuery::ViewPurchaseHome(Utility::DateTimeSpan {
                                                         QDateTime(QDate(QDate::currentDate().year(), 1, 1), QTime(0, 0)),
                                                         QDateTime::currentDateTime()
                                                     }, this));
}
