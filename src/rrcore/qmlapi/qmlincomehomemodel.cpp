#include "qmlincomehomemodel.h"
#include "database/databasethread.h"

QMLIncomeHomeModel::QMLIncomeHomeModel(QObject *parent) :
    QMLIncomeHomeModel(DatabaseThread::instance(), parent)
{}

QMLIncomeHomeModel::QMLIncomeHomeModel(DatabaseThread &thread, QObject *parent) :
    AbstractHomeModel(thread, parent)
{

}

void QMLIncomeHomeModel::tryQuery()
{

}
