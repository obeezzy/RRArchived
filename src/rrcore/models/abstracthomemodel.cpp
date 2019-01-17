#include "abstracthomemodel.h"

AbstractHomeModel::AbstractHomeModel(QObject *parent) :
    AbstractVisualListModel(parent)
{

}

AbstractHomeModel::AbstractHomeModel(DatabaseThread &thread) :
    AbstractVisualListModel(thread)
{

}

QVariantList AbstractHomeModel::records() const
{
    return m_records;
}

int AbstractHomeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_records.count();
}

QVariant AbstractHomeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case TitleRole:
        return m_records.at(index.row()).toMap().value("title").toString();
    case ImageUrlRole:
        return m_records.at(index.row()).toMap().value("image_url").toString();
    case ShortDescriptionRole:
        return m_records.at(index.row()).toMap().value("short_description").toString();
    case LongDescriptionRole:
        return m_records.at(index.row()).toMap().value("long_description").toString();
    }

    return QVariant();
}

QHash<int, QByteArray> AbstractHomeModel::roleNames() const
{
    return {
        { TitleRole, "title" },
        { ImageUrlRole, "image_url" },
        { ShortDescriptionRole, "short_description" },
        { LongDescriptionRole, "long_description" }
    };
}

void AbstractHomeModel::processResult(const QueryResult result)
{
    if (this != result.request().receiver())
        return;

    setBusy(false);
    beginResetModel();
    if (result.isSuccessful()) {
        m_records = result.outcome().toMap().value("records").toList();
        emit success();
    } else {
        emit error();
    }
    endResetModel();
}
