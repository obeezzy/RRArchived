#ifndef QMLEXPENSEPUSHER_H
#define QMLEXPENSEPUSHER_H

#include "pusher/abstractpusher.h"

class QMLExpensePusher : public AbstractPusher
{
    Q_OBJECT
    Q_PROPERTY(QString clientName READ clientName WRITE setClientName NOTIFY clientNameChanged)
    Q_PROPERTY(QString purpose READ purpose WRITE setPurpose NOTIFY purposeChanged)
    Q_PROPERTY(qreal amountPaid READ amountPaid WRITE setAmountPaid NOTIFY amountPaidChanged)
    Q_PROPERTY(PaymentMethod paymentMethod READ paymentMethod WRITE setPaymentMethod NOTIFY paymentMethodChanged)
public:
    enum class PaymentMethod {
        Cash, DebitCard, CreditCard
    }; Q_ENUM(PaymentMethod)

    enum class SuccessCode {
        AddExpenseSuccess,
        UpdateExpenseSuccess
    }; Q_ENUM(SuccessCode)

    enum class ErrorCode {
        AddExpenseError
    }; Q_ENUM(ErrorCode)

    explicit QMLExpensePusher(QObject *parent = nullptr);
    explicit QMLExpensePusher(DatabaseThread &thread);

    QString clientName() const;
    void setClientName(const QString &clientName);

    QString purpose() const;
    void setPurpose(const QString &purpose);

    qreal amountPaid() const;
    void setAmountPaid(qreal amountPaid);

    PaymentMethod paymentMethod() const;
    void setPaymentMethod(PaymentMethod paymentMethod);
signals:
    void clientNameChanged();
    void purposeChanged();
    void amountPaidChanged();
    void paymentMethodChanged();
public slots:
    void push() override;
protected:
    void processResult(const QueryResult result) override;
private:
    QString m_clientName;
    QString m_purpose;
    qreal m_amountPaid;
    PaymentMethod m_paymentMethod;
};

#endif // QMLEXPENSEPUSHER_H
