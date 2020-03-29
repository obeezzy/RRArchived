#ifndef QMLEXPENSEPUSHER_H
#define QMLEXPENSEPUSHER_H

#include "pusher/abstractpusher.h"
#include "utility/expense/expensetransaction.h"
#include <QLoggingCategory>

class QMLExpensePusher : public AbstractPusher
{
    Q_OBJECT
    Q_PROPERTY(QString clientName READ clientName WRITE setClientName NOTIFY clientNameChanged)
    Q_PROPERTY(QString purpose READ purpose WRITE setPurpose NOTIFY purposeChanged)
    Q_PROPERTY(double amount READ amount WRITE setAmount NOTIFY amountChanged)
    Q_PROPERTY(PaymentMethod paymentMethod READ paymentMethod WRITE setPaymentMethod NOTIFY paymentMethodChanged)
public:
    enum class PaymentMethod {
        Cash, DebitCard, CreditCard
    }; Q_ENUM(PaymentMethod)

    enum SuccessCode {
        AddExpenseSuccess,
        UpdateExpenseSuccess
    }; Q_ENUM(SuccessCode)

    enum ErrorCode {
        AddExpenseError
    }; Q_ENUM(ErrorCode)

    explicit QMLExpensePusher(QObject *parent = nullptr);
    explicit QMLExpensePusher(DatabaseThread &thread, QObject *parent = nullptr);

    QString clientName() const;
    void setClientName(const QString &clientName);

    QString purpose() const;
    void setPurpose(const QString &purpose);

    double amount() const;
    void setAmount(double amount);

    PaymentMethod paymentMethod() const;
    void setPaymentMethod(PaymentMethod paymentMethod);

    void push() override;
signals:
    void clientNameChanged();
    void purposeChanged();
    void amountChanged();
    void paymentMethodChanged();
protected:
    void processResult(const QueryResult result) override;
private:
    Utility::ExpenseTransaction m_transaction;
};

Q_DECLARE_LOGGING_CATEGORY(lcqmlexpensepusher);

#endif // QMLEXPENSEPUSHER_H
