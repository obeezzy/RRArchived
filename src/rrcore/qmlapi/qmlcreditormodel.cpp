#include "qmlcreditormodel.h"

QMLCreditorModel::QMLCreditorModel(QObject *parent) :
    AbstractVisualListModel (parent)
{

}

QMLCreditorModel::QMLCreditorModel(DatabaseThread &thread) :
    AbstractVisualListModel (thread)
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
