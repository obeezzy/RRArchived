#include "qmlexpensehomemodel.h"
#include "database/databasethread.h"

QMLExpenseHomeModel::QMLExpenseHomeModel(QObject* parent)
    : QMLExpenseHomeModel(DatabaseThread::instance(), parent)
{}

QMLExpenseHomeModel::QMLExpenseHomeModel(DatabaseThread& thread,
                                         QObject* parent)
    : AbstractHomeModel(thread, parent)
{}

void QMLExpenseHomeModel::tryQuery() {}

bool QMLExpenseHomeModel::canProcessResult(const QueryResult& result) const
{
    Q_UNUSED(result)
    return true;
}

void QMLExpenseHomeModel::processResult(const QueryResult& result)
{
    Q_UNUSED(result)
}
