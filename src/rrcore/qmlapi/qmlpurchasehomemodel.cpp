#include "qmlpurchasehomemodel.h"
#include "database/databasethread.h"

QMLPurchaseHomeModel::QMLPurchaseHomeModel(QObject *parent) :
    QMLPurchaseHomeModel(DatabaseThread::instance(), parent)
{}

QMLPurchaseHomeModel::QMLPurchaseHomeModel(DatabaseThread &thread, QObject *parent) :
    AbstractHomeModel(thread, parent)
{

}

void QMLPurchaseHomeModel::tryQuery()
{
    setBusy(true);
    QueryRequest request(this);
    request.setCommand("view_purchase_home", { }, QueryRequest::Purchase);
    emit executeRequest(request);
}
