#include "abstracttransactionmodel.h"
#include "database/databasethread.h"
#include <QDateTime>

AbstractTransactionModel::AbstractTransactionModel(QObject *parent) :
    AbstractTransactionModel(DatabaseThread::instance(), parent)
{}

AbstractTransactionModel::AbstractTransactionModel(DatabaseThread &thread, QObject *parent) :
    AbstractVisualTableModel(thread, parent)
{

}

int AbstractTransactionModel::keys() const
{
    return m_keys;
}

void AbstractTransactionModel::setKeys(int keys)
{
    if (m_keys == keys)
        return;

    m_keys = keys;
    emit keysChanged();
}

QDateTime AbstractTransactionModel::from() const
{
    return m_dateTimeSpan.from;
}

void AbstractTransactionModel::setFrom(const QDateTime &from)
{
    if (m_dateTimeSpan.from == from)
        return;

    m_dateTimeSpan.from = from;
    emit fromChanged();
}

QDateTime AbstractTransactionModel::to() const
{
    return m_dateTimeSpan.to;
}

void AbstractTransactionModel::setTo(const QDateTime &to)
{
    if (m_dateTimeSpan.to == to)
        return;

    m_dateTimeSpan.to = to;
    emit toChanged();
}

void AbstractTransactionModel::componentComplete()
{
    toggleConnections();
    AbstractVisualTableModel::componentComplete();
}

Utility::DateTimeSpan AbstractTransactionModel::dateTimeSpan() const
{
    return m_dateTimeSpan;
}

void AbstractTransactionModel::toggleConnections()
{
    if (autoQuery()) {
        connect(this, &AbstractTransactionModel::fromChanged, this, &AbstractTransactionModel::tryQuery);
        connect(this, &AbstractTransactionModel::toChanged, this, &AbstractTransactionModel::tryQuery);
    } else {
        disconnect(this, &AbstractTransactionModel::fromChanged, this, &AbstractTransactionModel::tryQuery);
        disconnect(this, &AbstractTransactionModel::toChanged, this, &AbstractTransactionModel::tryQuery);
    }
}
