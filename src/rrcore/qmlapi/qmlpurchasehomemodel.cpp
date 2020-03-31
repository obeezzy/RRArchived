#include "qmlpurchasehomemodel.h"
#include "database/databasethread.h"
#include "queryexecutors/purchase.h"
#include "utility/commonutils.h"
#include <QDateTime>

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

bool QMLPurchaseHomeModel::canProcessResult(const QueryResult &result) const
{
    Q_UNUSED(result)
    return true;
}

void QMLPurchaseHomeModel::processResult(const QueryResult &result)
{
    Q_UNUSED(result)
}
