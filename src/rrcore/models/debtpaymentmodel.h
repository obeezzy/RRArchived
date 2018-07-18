#ifndef DEBTPAYMENTMODEL_H
#define DEBTPAYMENTMODEL_H

#include "abstractvisuallistmodel.h"

class DebtPaymentModel : public AbstractVisualListModel
{
public:
    enum Role {
        AmountPaidRole = Qt::UserRole,
        BalanceRole,
        CurrencyRole,
        DueDateRole,
        NoteRole,
        ArchivedRole,
        CreatedRole
    };
    explicit DebtPaymentModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    int transactionId() const;
    void setTransactionId(int transactionId);

    void setPayments(const QVariantList &payments);
    bool addPayment();
    bool removePayment();
protected:
    void tryQuery() override;
    void processResult(const QueryResult result) override;
private:
    int m_transactionId;
    QVariantList m_records;
};

#endif // DEBTPAYMENTMODEL_H
