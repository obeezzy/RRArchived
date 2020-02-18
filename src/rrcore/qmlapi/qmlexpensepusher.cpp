#include "qmlexpensepusher.h"
#include "database/databasethread.h"
#include "queryexecutors/expense.h"
#include "utility/commonutils.h"

Q_LOGGING_CATEGORY(lcqmlexpensepusher, "rrcore.qmlapi.qmlexpensepusher");

QMLExpensePusher::QMLExpensePusher(QObject *parent) :
    QMLExpensePusher(DatabaseThread::instance(), parent)
{}

QMLExpensePusher::QMLExpensePusher(DatabaseThread &thread,
                                   QObject *parent) :
    AbstractPusher(thread, parent)
{

}

QString QMLExpensePusher::clientName() const
{
    return m_transaction.client.preferredName;
}

void QMLExpensePusher::setClientName(const QString &clientName)
{
    if (m_transaction.client.preferredName == clientName)
        return;

    m_transaction.client.preferredName = clientName;
    emit clientNameChanged();
}

QString QMLExpensePusher::purpose() const
{
    return m_transaction.purpose;
}

void QMLExpensePusher::setPurpose(const QString &purpose)
{
    if (m_transaction.purpose == purpose)
        return;

    m_transaction.purpose = purpose;
    emit purposeChanged();
}

qreal QMLExpensePusher::amount() const
{
    return m_transaction.amount;
}

void QMLExpensePusher::setAmount(qreal amount)
{
    if (m_transaction.amount == amount)
        return;

    m_transaction.amount = amount;
    emit amountChanged();
}

QMLExpensePusher::PaymentMethod QMLExpensePusher::paymentMethod() const
{
    switch (m_transaction.paymentMethod.enumValue()) {
    case Utility::PaymentMethod::DebitCard:
        return PaymentMethod::DebitCard;
    case Utility::PaymentMethod::CreditCard:
        return PaymentMethod::CreditCard;
    default:
        qCWarning(lcqmlexpensepusher) << "Invalid Utility::PaymentMethod enum value. "
                                      << "Defaulting to QMLExpensePusher::PaymentMethod::Cash...";
    }

    return PaymentMethod::Cash;
}

void QMLExpensePusher::setPaymentMethod(PaymentMethod paymentMethod)
{
    if (m_transaction.paymentMethod == Utility::PaymentMethod{ static_cast<int>(paymentMethod) })
        return;

    m_transaction.paymentMethod = Utility::PaymentMethod{ static_cast<int>(paymentMethod) };
    emit paymentMethodChanged();
}

void QMLExpensePusher::push()
{
    setBusy(true);
    emit execute(new ExpenseQuery::AddExpenseTransaction(m_transaction, this));
}

void QMLExpensePusher::processResult(const QueryResult result)
{
    if (this != result.request().receiver())
        return;

    setBusy(false);

    if (result.isSuccessful()) {
        if (result.request().command() == ExpenseQuery::AddExpenseTransaction::COMMAND)
            emit success(ModelResult{ AddExpenseSuccess });
    } else {
        emit error();
    }
}
