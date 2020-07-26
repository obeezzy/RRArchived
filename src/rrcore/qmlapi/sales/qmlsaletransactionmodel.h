#ifndef QMLSALETRANSACTIONMODEL_H
#define QMLSALETRANSACTIONMODEL_H

#include "models/abstracttransactionmodel.h"
#include "utility/sales/saletransaction.h"

class QMLSaleTransactionModel : public AbstractTransactionModel
{
    Q_OBJECT
public:
    enum SuccessCode
    {
        ViewTransactionSuccess,
        RemoveTransactionSuccess,
        UndoRemoveTransactionSuccess
    };
    Q_ENUM(SuccessCode)

    enum ErrorCode
    {
        ViewTransactionError,
        RemoveTransactionError,
        UndoRemoveTransactionError
    };
    Q_ENUM(ErrorCode)

    enum Columns
    {
        TransactionIdColumn,
        CustomerNameColumn,
        TotalCostColumn,
        ActionColumn,
        ColumnCount
    };
    Q_ENUM(Columns)

    enum Roles
    {
        SaleTransactionIdRole = Qt::UserRole,
        CustomerIdRole,
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
        UserIdRole,
        UserRole
    };

    explicit QMLSaleTransactionModel(QObject* parent = nullptr);
    explicit QMLSaleTransactionModel(DatabaseThread& thread,
                                     QObject* parent = nullptr);

    QVariant data(const QModelIndex& index,
                  int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

protected:
    void tryQuery() override;
    bool canProcessResult(const QueryResult& result) const override;
    void processResult(const QueryResult& result) override;

private:
    Utility::Sales::SaleTransactionList m_transactions;
};

#endif  // QMLSALETRANSACTIONMODEL_H
