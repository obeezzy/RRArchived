#ifndef QMLSALETRANSACTIONMODEL_H
#define QMLSALETRANSACTIONMODEL_H

#include "models/abstracttransactionmodel.h"

class QMLSaleTransactionModel : public AbstractTransactionModel
{
    Q_OBJECT
public:
    enum SuccessCode {
        UnknownSuccess,
        ViewTransactionSuccess,
        RemoveTransactionSuccess,
        UndoRemoveTransactionSuccess
    }; Q_ENUM(SuccessCode)

    enum ErrorCode {
        UnknownError,
        ViewTransactionError,
        RemoveTransactionError,
        UndoRemoveTransactionError
    }; Q_ENUM(ErrorCode)

    enum Columns {
        TransactionIdColumn,
        CustomerNameColumn,
        TotalCostColumn,
        ActionColumn,
        ColumnCount
    }; Q_ENUM(Columns)

    enum Roles {
        TransactionIdRole = Qt::UserRole,
        ClientIdRole,
        CustomerNameRole,
        TotalCostRole,
        AmountPaidRole,
        BalanceRole,
        DiscountRole,
        NoteIdRole,
        NoteRole,
        SuspendedRole,
        ArchivedRole,
        CreatedRole,
        LastEditedRole,
        UserIdRole
    };

    explicit QMLSaleTransactionModel(QObject *parent = nullptr);
    explicit QMLSaleTransactionModel(DatabaseThread &thread, QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
protected:
    void tryQuery() override;
    void processResult(const QueryResult result) override;
public slots:
    void removeTransaction(int row);
private:
    QVariantList m_records;
};

#endif // QMLSALETRANSACTIONMODEL_H
