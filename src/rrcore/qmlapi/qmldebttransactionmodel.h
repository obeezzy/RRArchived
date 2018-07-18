#ifndef QMLDEBTTRANSACTIONMODEL_H
#define QMLDEBTTRANSACTIONMODEL_H

#include "models/abstractvisuallistmodel.h"

class DebtPaymentModel;

class QMLDebtTransactionModel : public AbstractVisualListModel
{
    Q_OBJECT
    Q_PROPERTY(int debtorId READ debtorId WRITE setDebtorId NOTIFY debtorIdChanged)
public:
    enum SuccessCode {
        UnknownSuccess
    }; Q_ENUM(SuccessCode)

    enum ErrorCode {
        UnknownError
    }; Q_ENUM(ErrorCode)

    enum Role {
        TransactionIdRole = Qt::UserRole,
        RelatedTransactionRole,
        RelatedTransactionIdRole,
        DueDateRole,
        NoteRole,
        CreatedRole,
        PaymentModelRole,
        CurrentBalanceRole
    };

    explicit QMLDebtTransactionModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    int debtorId() const;
    void setDebtorId(int debtorId);

    Q_INVOKABLE void clearPayments();
signals:
    void debtorIdChanged();
protected:
    void tryQuery() override;
    void processResult(const QueryResult result) override;
private:
    int m_debtorId;
    QVariantList m_records;
    QList<DebtPaymentModel *> m_debtPaymentModels;
};

#endif // QMLDEBTTRANSACTIONMODEL_H
