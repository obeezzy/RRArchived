#include "qmlpurchasehomemodel.h"

QMLPurchaseHomeModel::QMLPurchaseHomeModel(QObject *parent) :
    AbstractHomeModel (parent)
{

}

QMLPurchaseHomeModel::QMLPurchaseHomeModel(DatabaseThread &thread) :
    AbstractHomeModel (thread)
{

}

void QMLPurchaseHomeModel::tryQuery()
{
    setBusy(true);
    QueryRequest request(this);
    request.setCommand("view_purchase_home", { }, QueryRequest::Purchase);
    emit executeRequest(request);
}
