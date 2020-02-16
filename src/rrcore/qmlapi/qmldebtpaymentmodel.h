#ifndef QMLDEBTPAYMENTMODEL_H
#define QMLDEBTPAYMENTMODEL_H

#include "models/abstractvisuallistmodel.h"
#include "utility/debtor/debtpayment.h"

class DatabaseThread;

class QMLDebtPaymentModel : public AbstractVisualListModel
{
    Q_OBJECT
    Q_PROPERTY(int debtTransactionId READ debtTransactionId WRITE setDebtTransactionId NOTIFY debtTransactionIdChanged)
    Q_PROPERTY(QVariant debtTransactionRef READ debtTransactionRef WRITE setDebtTransactionRef NOTIFY debtTransactionRefChanged)
    Q_PROPERTY(qreal totalAmountPaid READ totalAmountPaid NOTIFY totalAmountPaidChanged)
public:
    enum Role {
        AmountOwedRole = Qt::UserRole,
        AmountPaidRole,
        BalanceRole,
        MaxPayableAmount,
        CurrencyRole,
        DueDateRole,
        NoteRole,
        DirtyRole,
        FreshRole,
        ArchivedRole,
        CreatedRole,
        LastEditedRole
    };

    enum SuccessCode {
        ViewDebtorPaymentsSuccess,
    }; Q_ENUM(SuccessCode)

    enum ErrorCode {
        UnknownError
    }; Q_ENUM(ErrorCode)

    explicit QMLDebtPaymentModel(QObject *parent = nullptr);
    explicit QMLDebtPaymentModel(DatabaseThread &thread, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    int debtTransactionId() const;
    void setDebtTransactionId(int debtTransactionId);

    QVariant debtTransactionRef() const;
    void setDebtTransactionRef(const QVariant &debtTransactionRef);

    qreal totalAmountPaid() const;
    void setTotalAmountPaid(qreal totalAmountPaid);

    Q_INVOKABLE void addPayment(double amount,
                                const QString &note = QString());
    Q_INVOKABLE void removePayment(int row);
signals:
    void debtTransactionIdChanged();
    void debtTransactionRefChanged();
    void totalAmountPaidChanged();
protected:
    void tryQuery() override;
    void processResult(const QueryResult result) override;
private:
    int m_debtTransactionId {-1};
    qreal m_totalAmountPaid {0.0};
    QVariant m_debtTransactionRef;
    Utility::DebtPaymentList m_payments;

    void calculateTotals();
    void updateRef(Utility::DebtPayment payment);
};

Q_DECLARE_LOGGING_CATEGORY(lcqmldebtpaymentmodel);

#endif // QMLDEBTPAYMENTMODEL_H
