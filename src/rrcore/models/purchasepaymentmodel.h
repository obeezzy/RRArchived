#ifndef PURCHASEPAYMENTMODEL_H
#define PURCHASEPAYMENTMODEL_H

#include "abstractvisuallistmodel.h"
#include "utility/purchaseutils.h"

class PurchasePaymentModel : public AbstractVisualListModel
{
    Q_OBJECT
public:
    enum Roles {
        AmountRole = Qt::UserRole,
        MethodRole,
        NoteRole
    }; Q_ENUM(Roles)

    explicit PurchasePaymentModel(QObject *parent = nullptr);
    ~PurchasePaymentModel() override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override final;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override final;
    QHash<int, QByteArray> roleNames() const override final;

    void addPayment(const PurchasePayment &payment);
    void removePayment(int index);
    void clearPayments();

    int cashPaymentCount() const;
    int cardPaymentCount() const;

    void calculatePaymentCount();
protected:
    void tryQuery() override;
    void processResult(const QueryResult result) override;
signals:
    void cashPaymentCountChanged();
    void cardPaymentCountChanged();
private:
    PurchasePaymentList m_purchasePayments;
    int m_cashPaymentCount;
    int m_cardPaymentCount;
}; Q_DECLARE_METATYPE(PurchasePaymentModel *)

#endif // PURCHASEPAYMENTMODEL_H
