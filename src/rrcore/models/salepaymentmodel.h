#ifndef SALEPAYMENTMODEL_H
#define SALEPAYMENTMODEL_H

#include "models/abstractvisuallistmodel.h"

class SalePayment;

class SalePaymentModel : public AbstractVisualListModel
{
    Q_OBJECT
public:
    enum Roles {
        AmountRole = Qt::UserRole,
        MethodRole,
        NoteRole
    }; Q_ENUM(Roles)

    explicit SalePaymentModel(QObject *parent = nullptr);
    ~SalePaymentModel() override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override final;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override final;
    QHash<int, QByteArray> roleNames() const override final;

    void addPayment(SalePayment payment);
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
    QList<SalePayment> m_salePayments;
    int m_cashPaymentCount;
    int m_cardPaymentCount;
}; Q_DECLARE_METATYPE(SalePaymentModel *)

#endif // SALEPAYMENTMODEL_H
