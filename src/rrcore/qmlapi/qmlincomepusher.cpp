#include "qmlincomepusher.h"

QMLIncomePusher::QMLIncomePusher(QObject *parent) :
    AbstractPusher (parent)
{

}

QMLIncomePusher::QMLIncomePusher(DatabaseThread &thread) :
    AbstractPusher (thread)
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

void QMLIncomePusher::push()
{

}

void QMLIncomePusher::processResult(const QueryResult result)
{
    if (this != result.request().receiver())
        return;
}
