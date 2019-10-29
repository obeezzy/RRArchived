#include "receiptcartmodel.h"
#include <QVariantMap>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

const int COLUMN_COUNT = 4;

ReceiptCartModel::ReceiptCartModel(QObject *parent) :
    AbstractVisualTableModel (parent)
{

}

ReceiptCartModel::ReceiptCartModel(DatabaseThread &thread) :
    AbstractVisualTableModel (thread)
{

}

QVariant ReceiptCartModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case QuantityRole:
        return m_records.at(index.row()).toMap().value("quantity").toDouble();
    case ItemRole:
        return m_records.at(index.row()).toMap().value("item").toString();
    case PriceRole:
        return m_records.at(index.row()).toMap().value("price").toDouble();
    case TotalRole:
        return m_records.at(index.row()).toMap().value("total").toDouble();
    }

    return QVariant();
}

int ReceiptCartModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_records.count();
}

int ReceiptCartModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return COLUMN_COUNT;
}

QHash<int, QByteArray> ReceiptCartModel::roleNames() const
{
    return {
        { QuantityRole, "quantity" },
        { ItemRole, "item" },
        { PriceRole, "price" },
        { TotalRole, "total" }
    };
}

void ReceiptCartModel::setRecords(const QString &json)
{
    if (json.trimmed().isEmpty())
        return;

    m_records = QJsonDocument::fromJson(json.toUtf8()).object().value("records").toArray().toVariantList();
}

QString ReceiptCartModel::paymentType() const
{
    return QStringLiteral("Cash");
}

void ReceiptCartModel::tryQuery()
{

}

void ReceiptCartModel::processResult(const QueryResult result)
{
    Q_UNUSED(result)
}

QString ReceiptCartModel::columnName(int column) const
{
    switch (static_cast<Columns>(column)) {
    case Columns::QuantityColumn:
        return "quantity";
    case Columns::ItemColumn:
        return "item";
    case Columns::PriceColumn:
        return "price";
    case Columns::TotalColumn:
        return "total";
    }

    return QString();
}
