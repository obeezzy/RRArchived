#include "qmlincomepusher.h"
#include "database/databasethread.h"

QMLIncomePusher::QMLIncomePusher(QObject *parent) :
    QMLIncomePusher(DatabaseThread::instance(), parent)
{}

QMLIncomePusher::QMLIncomePusher(DatabaseThread &thread, QObject *parent) :
    AbstractPusher(thread, parent),
    m_paymentMethod(PaymentMethod::Cash)
{

}

QString QMLIncomePusher::clientName() const
{
    return m_clientName;
}

void QMLIncomePusher::setClientName(const QString &clientName)
{
    if (m_clientName == clientName)
        return;

    m_clientName = clientName;
    emit clientNameChanged();
}

QString QMLIncomePusher::purpose() const
{
    return m_purpose;
}

void QMLIncomePusher::setPurpose(const QString &purpose)
{
    if (m_purpose == purpose)
        return;

    m_purpose = purpose;
    emit purposeChanged();
}

qreal QMLIncomePusher::amount() const
{
    return m_amount;
}

void QMLIncomePusher::setAmount(qreal amount)
{
    if (m_amount == amount)
        return;

    m_amount = amount;
    emit amountChanged();
}

QMLIncomePusher::PaymentMethod QMLIncomePusher::paymentMethod() const
{
    return m_paymentMethod;
}

void QMLIncomePusher::setPaymentMethod(QMLIncomePusher::PaymentMethod paymentMethod)
{
    if (m_paymentMethod == paymentMethod)
        return;

    m_paymentMethod = paymentMethod;
    emit paymentMethodChanged();
}

void QMLIncomePusher::push()
{
    setBusy(true);

    QString paymentMethod;
    switch (m_paymentMethod) {
    case PaymentMethod::Cash:
        paymentMethod = "cash";
        break;
    case PaymentMethod::DebitCard:
        paymentMethod = "debit_card";
        break;
    case PaymentMethod::CreditCard:
        paymentMethod = "credit_card";
        break;
    }

    QVariantMap params {
        { "client_name", m_clientName },
        { "purpose", m_purpose },
        { "amount", m_amount },
        { "payment_method", paymentMethod }
    };

    QueryRequest request(this);
    request.setCommand("add_new_income_transaction", params, QueryRequest::Income);
    emit executeRequest(request);
}

void QMLIncomePusher::processResult(const QueryResult result)
{
    if (this != result.request().receiver())
        return;

    setBusy(false);

    if (result.isSuccessful()) {
        if (result.request().command() == "add_new_income_transaction")
            emit success(static_cast<int>(SuccessCode::AddIncomeSuccess));
    } else {
        emit error();
    }
}
