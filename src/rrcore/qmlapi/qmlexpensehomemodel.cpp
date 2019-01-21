#include "qmlexpensehomemodel.h"

QMLExpenseHomeModel::QMLExpenseHomeModel(QObject *parent) :
    AbstractHomeModel (parent)
{

}

QMLExpenseHomeModel::QMLExpenseHomeModel(DatabaseThread &thread) :
    AbstractHomeModel (thread)
{

}

void QMLExpenseHomeModel::tryQuery()
{

}
