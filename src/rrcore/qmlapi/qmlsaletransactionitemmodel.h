#ifndef QMLSALETRANSACTIONITEMMODEL_H
#define QMLSALETRANSACTIONITEMMODEL_H

#include "models/abstracttransactionitemmodel.h"
#include <QVariantList>

class QMLSaleTransactionItemModel : public AbstractTransactionItemModel
{
    Q_OBJECT
public:
    enum SuccessCode {
        UnknownSuccess,
        ViewSaleTransactionItemsSuccess
    }; Q_ENUM(SuccessCode)

    enum ErrorCode {
        UnknownError
    }; Q_ENUM(ErrorCode)

    enum Roles {
        TransactionItemIdRole = Qt::UserRole,
        CategoryIdRole,
        CategoryRole,
        ItemIdRole,
        UnitPriceRole,
        ItemRole,
        QuantityRole,
        UnitIdRole,
        UnitRole,
        CostRole,
        DiscountRole,
        CurrencyRole,
        NoteIdRole,
        NoteRole,
        SuspendedRole,
        ArchivedRole,
        CreatedRole,
        LastEditedRole,
        UserIdRole,
        UserRole
    };

    enum Columns {
        CategoryColumn,
        ItemColumn,
        QuantityColumn,
        UnitPriceColumn,
        CostColumn,
        ColumnCount
    }; Q_ENUM(Columns)

    explicit QMLSaleTransactionItemModel(QObject *parent = nullptr);
    explicit QMLSaleTransactionItemModel(DatabaseThread &thread, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override final;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override final;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override final;
    QHash<int, QByteArray> roleNames() const override final;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
protected:
    void tryQuery() override final;
    void processResult(const QueryResult result) override final;
    QString columnName(int column) const override;
public slots:
    void removeTransactionItem(int row);
private:
    QVariantList m_records;
};

#endif // QMLSALETRANSACTIONITEMMODEL_H
