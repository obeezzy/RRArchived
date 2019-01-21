#include "qmlexpensepusher.h"

QMLExpensePusher::QMLExpensePusher(QObject *parent) :
    AbstractPusher (parent)
{

}

QMLExpensePusher::QMLExpensePusher(DatabaseThread &thread) :
    AbstractPusher (thread)
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

qreal QMLExpensePusher::amountPaid() const
{
    return m_amountPaid;
}

void QMLExpensePusher::setAmountPaid(qreal amountPaid)
{
    if (m_amountPaid == amountPaid)
        return;

    m_amountPaid = amountPaid;
    emit amountPaidChanged();
}

void QMLExpensePusher::push()
{

}

void QMLExpensePusher::processResult(const QueryResult result)
{
    if (this != result.request().receiver())
        return;
}
