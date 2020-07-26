#include "qmlincomepusher.h"
#include "database/databasethread.h"
#include "query/income/addincometransaction.h"

Q_LOGGING_CATEGORY(lcqmlincomepusher, "rrcore.qmlapi.qmlincomepusher")

QMLIncomePusher::QMLIncomePusher(QObject* parent)
    : QMLIncomePusher(DatabaseThread::instance(), parent)
{}

QMLIncomePusher::QMLIncomePusher(DatabaseThread& thread, QObject* parent)
    : AbstractPusher(thread, parent)
{}

QString QMLIncomePusher::clientName() const
{
    return m_transaction.client.preferredName;
}

void QMLIncomePusher::setClientName(const QString& clientName)
{
    if (m_transaction.client.preferredName == clientName)
        return;

    m_transaction.client.preferredName = clientName;
    emit clientNameChanged();
}

QString QMLIncomePusher::purpose() const
{
    return m_transaction.purpose;
}

void QMLIncomePusher::setPurpose(const QString& purpose)
{
    if (m_transaction.purpose == purpose)
        return;

    m_transaction.purpose = purpose;
    emit purposeChanged();
}

double QMLIncomePusher::amount() const
{
    return m_transaction.amount.toDouble();
}

void QMLIncomePusher::setAmount(double amount)
{
    if (m_transaction.amount == Utility::Money(amount))
        return;

    m_transaction.amount = Utility::Money(amount);
    emit amountChanged();
}

QMLIncomePusher::PaymentMethod QMLIncomePusher::paymentMethod() const
{
    if (m_transaction.paymentMethod == Utility::PaymentMethod::DebitCard)
        return PaymentMethod::DebitCard;
    else if (m_transaction.paymentMethod == Utility::PaymentMethod::CreditCard)
        return PaymentMethod::CreditCard;
    else if (m_transaction.paymentMethod == Utility::PaymentMethod::Cash)
        return PaymentMethod::Cash;
    else
        qCWarning(lcqmlincomepusher) << "Invalid PaymentMethod";

    return PaymentMethod::Cash;
}

void QMLIncomePusher::setPaymentMethod(
    QMLIncomePusher::PaymentMethod paymentMethod)
{
    if (m_transaction.paymentMethod ==
        Utility::PaymentMethod{static_cast<int>(paymentMethod)})
        return;

    m_transaction.paymentMethod =
        Utility::PaymentMethod{static_cast<int>(paymentMethod)};
    emit paymentMethodChanged();
}

void QMLIncomePusher::push()
{
    setBusy(true);
    emit execute(new Query::Income::AddIncomeTransaction(m_transaction, this));
}

bool QMLIncomePusher::canProcessResult(const QueryResult& result)
{
    Q_UNUSED(result)
    return true;
}

void QMLIncomePusher::processResult(const QueryResult result)
{
    setBusy(false);

    if (result.isSuccessful()) {
        if (result.request().command() ==
            Query::Income::AddIncomeTransaction::COMMAND)
            emit success(ModelResult{AddIncomeSuccess});
    } else {
        emit error();
    }
}
