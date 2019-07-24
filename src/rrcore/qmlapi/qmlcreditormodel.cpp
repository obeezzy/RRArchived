#include "qmlcreditormodel.h"
#include "database/databasethread.h"

QMLCreditorModel::QMLCreditorModel(QObject *parent) :
    QMLCreditorModel(DatabaseThread::instance(), parent)
{}

QMLCreditorModel::QMLCreditorModel(DatabaseThread &thread, QObject *parent) :
    AbstractVisualListModel(thread, parent)
{

}

int QMLCreditorModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 0;
}

QVariant QMLCreditorModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role)
    if (!index.isValid())
        return QVariant();

    return QVariant();
}

QHash<int, QByteArray> QMLCreditorModel::roleNames() const
{
    return { };
}

void QMLCreditorModel::tryQuery()
{

}

void QMLCreditorModel::processResult(const QueryResult result)
{
    if (this != result.request().receiver())
        return;
}
