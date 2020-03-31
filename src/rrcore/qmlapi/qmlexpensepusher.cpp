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
{}

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

double QMLExpensePusher::amount() const
{
    return m_transaction.amount.toDouble();
}

void QMLExpensePusher::setAmount(double amount)
{
    if (m_transaction.amount == Utility::Money(amount))
        return;

    m_transaction.amount = Utility::Money(amount);
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

bool QMLExpensePusher::canProcessResult(const QueryResult &result)
{
    Q_UNUSED(result)
    return true;
}

void QMLExpensePusher::processResult(const QueryResult result)
{
    setBusy(false);

    if (result.isSuccessful()) {
        if (result.request().command() == ExpenseQuery::AddExpenseTransaction::COMMAND)
            emit success(ModelResult{ AddExpenseSuccess });
    } else {
        emit error();
    }
}
