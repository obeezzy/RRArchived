#ifndef QMLSALEPAYMENTMODEL_H
#define QMLSALEPAYMENTMODEL_H

#include "models/abstractvisuallistmodel.h"
#include "utility/sales/salepayment.h"

class DatabaseThread;

class QMLSalePaymentModel : public AbstractVisualListModel
{
    Q_OBJECT
public:
    enum Roles
    {
        AmountRole = Qt::UserRole,
        MethodRole,
        NoteRole
    };
    Q_ENUM(Roles)

    explicit QMLSalePaymentModel(QObject* parent = nullptr);
    explicit QMLSalePaymentModel(DatabaseThread& thread,
                                 QObject* parent = nullptr);
    ~QMLSalePaymentModel() override = default;

    int rowCount(
        const QModelIndex& parent = QModelIndex()) const override final;
    QVariant data(const QModelIndex& index,
                  int role = Qt::DisplayRole) const override final;
    QHash<int, QByteArray> roleNames() const override final;

    void addPayment(Utility::Sales::SalePayment payment);
    void removePayment(int index);
    void clearPayments();

    int cashPaymentCount() const;
    int cardPaymentCount() const;

protected:
    void tryQuery() override;
    bool canProcessResult(const QueryResult& result) const override;
    void processResult(const QueryResult& result) override;
signals:
    void totalCostChanged();
    void amountPaidChanged();
    void balanceChanged();
    void totalAmountChanged();
    void cashPaymentCountChanged();
    void cardPaymentCountChanged();

private:
    Utility::Sales::SalePaymentList m_payments;
    Utility::Money m_totalAmount;

    void setTotalAmount(double totalAmount);
    void calculateTotalAmount();
};

#endif  // QMLSALEPAYMENTMODEL_H
