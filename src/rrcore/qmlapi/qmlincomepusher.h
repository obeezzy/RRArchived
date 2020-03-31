#ifndef QMLINCOMEPUSHER_H
#define QMLINCOMEPUSHER_H

#include "pusher/abstractpusher.h"
#include "utility/income/incometransaction.h"
#include <QLoggingCategory>

class QMLIncomePusher : public AbstractPusher
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
        AddIncomeSuccess,
        UpdateIncomeSuccess
    }; Q_ENUM(SuccessCode)

    enum ErrorCode {
        AddIncomeError
    }; Q_ENUM(ErrorCode)

    explicit QMLIncomePusher(QObject *parent = nullptr);
    explicit QMLIncomePusher(DatabaseThread &thread, QObject *parent = nullptr);

    QString clientName() const;
    void setClientName(const QString &clientName);

    QString purpose() const;
    void setPurpose(const QString &purpose);

    double amount() const;
    void setAmount(double amount);

    PaymentMethod paymentMethod() const;
    void setPaymentMethod(PaymentMethod paymentMethod);
signals:
    void clientNameChanged();
    void purposeChanged();
    void amountChanged();
    void paymentMethodChanged();
public slots:
    void push() override;
protected:
    bool canProcessResult(const QueryResult &result) override;
    void processResult(const QueryResult result) override;
private:
    Utility::IncomeTransaction m_transaction;
};

Q_DECLARE_LOGGING_CATEGORY(lcqmlincomepusher)

#endif // QMLINCOMEPUSHER_H
