#ifndef QMLPURCHASEPAYMENTMODEL_H
#define QMLPURCHASEPAYMENTMODEL_H

#include "models/abstractvisuallistmodel.h"
#include "utility/purchaseutils.h"

class QMLPurchasePaymentModel : public AbstractVisualListModel
{
    Q_OBJECT
    Q_PROPERTY(double totalAmount READ totalAmount WRITE setTotalAmount NOTIFY totalAmountChanged)
public:
    enum Roles {
        AmountRole = Qt::UserRole,
        MethodRole,
        NoteRole
    }; Q_ENUM(Roles)

    explicit QMLPurchasePaymentModel(QObject *parent = nullptr);
    ~QMLPurchasePaymentModel() override = default;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override final;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override final;
    QHash<int, QByteArray> roleNames() const override final;

    double totalAmount() const;

    void addPayment(const Utility::PurchasePayment &payment);
    void removePayment(int index);
    void clearPayments();

    int cashPaymentCount() const;
    int cardPaymentCount() const;
protected:
    void tryQuery() override;
    bool canProcessResult(const QueryResult &result) const override;
    void processResult(const QueryResult &result) override;
signals:
    void totalAmountChanged();
private:
    Utility::PurchasePaymentList m_payments;
    Utility::Money m_totalAmount;

    void setTotalAmount(double totalAmount);
    void calculateTotalAmount();
};

#endif // QMLPURCHASEPAYMENTMODEL_H
