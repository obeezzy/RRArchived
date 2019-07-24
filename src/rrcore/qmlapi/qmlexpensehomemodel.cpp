#include "qmlexpensehomemodel.h"
#include "database/databasethread.h"

QMLExpenseHomeModel::QMLExpenseHomeModel(QObject *parent) :
    QMLExpenseHomeModel(DatabaseThread::instance(), parent)
{}

QMLExpenseHomeModel::QMLExpenseHomeModel(DatabaseThread &thread, QObject *parent) :
    AbstractHomeModel(thread, parent)
{

}

void QMLExpenseHomeModel::tryQuery()
{

}
