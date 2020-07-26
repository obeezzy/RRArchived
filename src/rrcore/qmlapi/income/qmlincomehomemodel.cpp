#include "qmlincomehomemodel.h"
#include "database/databasethread.h"

QMLIncomeHomeModel::QMLIncomeHomeModel(QObject* parent)
    : QMLIncomeHomeModel(DatabaseThread::instance(), parent)
{}

QMLIncomeHomeModel::QMLIncomeHomeModel(DatabaseThread& thread, QObject* parent)
    : AbstractHomeModel(thread, parent)
{}

void QMLIncomeHomeModel::tryQuery() {}

bool QMLIncomeHomeModel::canProcessResult(const QueryResult& result) const
{
    Q_UNUSED(result)
    return true;
}

void QMLIncomeHomeModel::processResult(const QueryResult& result)
{
    Q_UNUSED(result)
}
