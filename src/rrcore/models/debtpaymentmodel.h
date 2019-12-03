#ifndef DEBTPAYMENTMODEL_H
#define DEBTPAYMENTMODEL_H

#include "abstractvisuallistmodel.h"
#include "utility/debtorutils.h"

class DebtPaymentModel : public AbstractVisualListModel
{
    Q_OBJECT
    Q_PROPERTY(bool dirty READ isDirty NOTIFY dirtyChanged)
public:
    enum Role {
        AmountPaidRole = Qt::UserRole,
        BalanceRole,
        CurrencyRole,
        DueDateRole,
        NoteRole,
        UncommittedRole,
        ArchivedRole,
        CreatedRole
    };
    explicit DebtPaymentModel(QObject *parent = nullptr);

    bool isDirty() const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    int transactionId() const;
    void setTransactionId(int transactionId);

    void setTotalAmount(double totalAmount);
    void setPaymentRecords(const QVariantList &records);

    void addPayment(DebtPayment debtPayment);
    void updatePayment(DebtPayment debtPayment);
    void removePayment(DebtPayment debtPayment);
    void clearPayments();
signals:
    void dirtyChanged();
protected:
    void tryQuery() override;
    void processResult(const QueryResult result) override;
private:
    int m_transactionId;
    QVariantList m_records;
    DebtPaymentList m_debtPayments;
    bool m_dirty;
    double m_totalAmount;

    void setDirty(bool dirty);
    bool isExistingRecord(int row) const;
};

#endif // DEBTPAYMENTMODEL_H
