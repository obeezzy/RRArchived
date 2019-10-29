#ifndef RECEIPTCARTMODEL_H
#define RECEIPTCARTMODEL_H

#include "models/abstractvisualtablemodel.h"
#include <QVariantList>

class ReceiptCartModel : public AbstractVisualTableModel
{
    Q_OBJECT
public:
    enum Roles {
        QuantityRole = Qt::UserRole,
        ItemRole,
        PriceRole,
        TotalRole
    };
    enum class Columns {
        QuantityColumn,
        ItemColumn,
        PriceColumn,
        TotalColumn
    }; Q_ENUM(Columns)

    explicit ReceiptCartModel(QObject *parent = nullptr);
    explicit ReceiptCartModel(const ReceiptCartModel &other) = default;
    explicit ReceiptCartModel(DatabaseThread &thread);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setRecords(const QString &json);

    QString paymentType() const;
protected:
    void tryQuery() override;
    void processResult(const QueryResult result) override;
    QString columnName(int column) const override final;
private:
    QVariantList m_records;
}; //Q_DECLARE_METATYPE(ReceiptCartModel);

#endif // RECEIPTCARTMODEL_H
