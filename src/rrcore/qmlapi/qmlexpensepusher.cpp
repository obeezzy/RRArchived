#include "qmlexpensepusher.h"

QMLExpensePusher::QMLExpensePusher(QObject *parent) :
    AbstractPusher (parent),
    m_paymentMethod(PaymentMethod::Cash)
{

}

QMLExpensePusher::QMLExpensePusher(DatabaseThread &thread) :
    AbstractPusher (thread),
    m_paymentMethod(PaymentMethod::Cash)
{

}

QString QMLExpensePusher::clientName() const
{
    return m_clientName;
}

void QMLExpensePusher::setClientName(const QString &clientName)
{
    if (m_clientName == clientName)
        return;

    m_clientName = clientName;
    emit clientNameChanged();
}

QString QMLExpensePusher::purpose() const
{
    return m_purpose;
}

void QMLExpensePusher::setPurpose(const QString &purpose)
{
    if (m_purpose == purpose)
        return;

    m_purpose = purpose;
    emit purposeChanged();
}

qreal QMLExpensePusher::amount() const
{
    return m_amount;
}

void QMLExpensePusher::setAmount(qreal amount)
{
    if (m_amount == amount)
        return;

    m_amount = amount;
    emit amountChanged();
}

QMLExpensePusher::PaymentMethod QMLExpensePusher::paymentMethod() const
{
    return m_paymentMethod;
}

void QMLExpensePusher::setPaymentMethod(QMLExpensePusher::PaymentMethod paymentMethod)
{
    if (m_paymentMethod == paymentMethod)
        return;

    m_paymentMethod = paymentMethod;
    emit paymentMethodChanged();
}

void QMLExpensePusher::push()
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
    request.setCommand("add_new_expense_transaction", params, QueryRequest::Expense);
    emit executeRequest(request);
}

void QMLExpensePusher::processResult(const QueryResult result)
{
    if (this != result.request().receiver())
        return;

    setBusy(false);

    if (result.isSuccessful()) {
        if (result.request().command() == "add_new_expense_transaction")
            emit success(static_cast<int>(SuccessCode::AddExpenseSuccess));
    } else {
        emit error();
    }
}
