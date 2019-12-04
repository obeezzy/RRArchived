#include "qmlincomepusher.h"
#include "database/databasethread.h"
#include "queryexecutors/income.h"

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
    emit execute(new IncomeQuery::AddIncomeTransaction(
                     m_clientName,
                     m_purpose,
                     m_amount,
                     paymentMethodAsString(),
                     this));
}

void QMLIncomePusher::processResult(const QueryResult result)
{
    if (this != result.request().receiver())
        return;

    setBusy(false);

    if (result.isSuccessful()) {
        if (result.request().command() == IncomeQuery::AddIncomeTransaction::COMMAND)
            emit success(AddIncomeSuccess);
    } else {
        emit error();
    }
}

QString QMLIncomePusher::paymentMethodAsString() const
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
