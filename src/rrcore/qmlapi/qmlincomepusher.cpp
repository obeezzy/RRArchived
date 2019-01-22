#include "qmlincomepusher.h"

QMLIncomePusher::QMLIncomePusher(QObject *parent) :
    AbstractPusher (parent),
    m_paymentMethod(PaymentMethod::Cash)
{

}

QMLIncomePusher::QMLIncomePusher(DatabaseThread &thread) :
    AbstractPusher (thread),
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

qreal QMLIncomePusher::amountPaid() const
{
    return m_amountPaid;
}

void QMLIncomePusher::setAmountPaid(qreal amountPaid)
{
    if (m_amountPaid == amountPaid)
        return;

    m_amountPaid = amountPaid;
    emit amountPaidChanged();
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
        { "amount_paid", m_amountPaid },
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
