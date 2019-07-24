#ifndef QMLPURCHASETRANSACTIONMODEL_H
#define QMLPURCHASETRANSACTIONMODEL_H

#include "models/abstracttransactionmodel.h"

class QMLPurchaseTransactionModel : public AbstractTransactionModel
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

    enum FilterColumn {
        TransactionColumn,
        ClientIdColumn,
        CustomerNameColumn,
        TotalCostColumn,
        AmountPaidColumn,
        BalanceColumn,
        DiscountColumn,
        NoteIdColumn,
        NoteColumn,
        SuspendedColumn,
        ArchivedColumn,
        CreatedColumn,
        LastEditedColumn,
        UserIdColumn
    }; Q_ENUM(FilterColumn)

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

    explicit QMLPurchaseTransactionModel(QObject *parent = nullptr);
    explicit QMLPurchaseTransactionModel(DatabaseThread &thread, QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const override;
protected:
    void tryQuery() override;
    void processResult(const QueryResult result) override;
public slots:
    void removeTransaction(int row);
private:
    QVariantList m_records;

    void removeTransactionFromModel(int row);
    void undoRemoveTransactionFromModel(int row, const QVariantMap &record);
};

#endif // QMLPURCHASETRANSACTIONMODEL_H
