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
        ProductRole,
        PriceRole,
        TotalRole
    };
    enum class Columns {
        QuantityColumn,
        ProductColumn,
        PriceColumn,
        TotalColumn
    }; Q_ENUM(Columns)

    explicit ReceiptCartModel(QObject *parent = nullptr);
    explicit ReceiptCartModel(const ReceiptCartModel &other) = delete;
    explicit ReceiptCartModel(DatabaseThread &thread);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setRecords(const QString &json);

    QString paymentType() const;
protected:
    void tryQuery() override;
    bool canProcessResult(const QueryResult &result) const override;
    void processResult(const QueryResult &result) override final;
    QString columnName(int column) const override final;
private:
    QVariantList m_records;
}; //Q_DECLARE_METATYPE(ReceiptCartModel);

#endif // RECEIPTCARTMODEL_H
