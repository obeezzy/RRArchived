#include "qmlincomehomemodel.h"

QMLIncomeHomeModel::QMLIncomeHomeModel(QObject *parent) :
    AbstractHomeModel (parent)
{

}

QMLIncomeHomeModel::QMLIncomeHomeModel(DatabaseThread &thread) :
    AbstractHomeModel (thread)
{

}

void QMLIncomeHomeModel::tryQuery()
{

}
