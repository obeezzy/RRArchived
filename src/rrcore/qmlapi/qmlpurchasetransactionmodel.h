#ifndef QMLPURCHASETRANSACTIONMODEL_H
#define QMLPURCHASETRANSACTIONMODEL_H

#include "models/abstracttransactionmodel.h"
#include "utility/purchase/purchasetransaction.h"

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

    explicit QMLPurchaseTransactionModel(QObject *parent = nullptr);
    explicit QMLPurchaseTransactionModel(DatabaseThread &thread, QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const override;
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
protected:
    void tryQuery() override;
    bool canProcessResult(const QueryResult &result) const override final;
    void processResult(const QueryResult &result) override final;
public slots:
    void removeTransaction(int row);
private:
    Utility::PurchaseTransactionList m_transactions;

    void removeTransactionFromModel(const Utility::PurchaseTransaction &transaction);
    void undoRemoveTransactionFromModel(const Utility::PurchaseTransaction &transaction);
};

#endif // QMLPURCHASETRANSACTIONMODEL_H
