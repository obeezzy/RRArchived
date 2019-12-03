#include "qmlexpensepusher.h"
#include "database/databasethread.h"
#include "queryexecutors/expense.h"

QMLExpensePusher::QMLExpensePusher(QObject *parent) :
    QMLExpensePusher(DatabaseThread::instance(), parent)
{}

QMLExpensePusher::QMLExpensePusher(DatabaseThread &thread, QObject *parent) :
    AbstractPusher(thread, parent),
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
    emit execute(new ExpenseQuery::AddExpenseTransaction(
                     m_clientName,
                     m_purpose,
                     m_amount,
                     paymentMethodAsString(),
                     this));
}

void QMLExpensePusher::processResult(const QueryResult result)
{
    if (this != result.request().receiver())
        return;

    setBusy(false);

    if (result.isSuccessful()) {
        if (result.request().command() == ExpenseQuery::AddExpenseTransaction::COMMAND)
            emit success(static_cast<int>(SuccessCode::AddExpenseSuccess));
    } else {
        emit error();
    }
}

QString QMLExpensePusher::paymentMethodAsString() const
{
    switch (m_paymentMethod) {
    case PaymentMethod::Cash:
        return QStringLiteral("cash");
    case PaymentMethod::DebitCard:
        return QStringLiteral("debit_card");
    case PaymentMethod::CreditCard:
        return QStringLiteral("credit_card");
    }

    return QString();
}
