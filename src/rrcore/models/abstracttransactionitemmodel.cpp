#include "abstracttransactionitemmodel.h"
#include "database/databasethread.h"

AbstractTransactionItemModel::AbstractTransactionItemModel(QObject* parent)
    : AbstractTransactionItemModel(DatabaseThread::instance(), parent)
{}

AbstractTransactionItemModel::AbstractTransactionItemModel(
    DatabaseThread& thread, QObject* parent)
    : AbstractVisualTableModel(thread, parent)
{
    connect(this, &AbstractTransactionItemModel::transactionIdChanged,
            &AbstractTransactionItemModel::tryQuery);
}

int AbstractTransactionItemModel::transactionId() const
{
    return m_transactionId;
}

void AbstractTransactionItemModel::setTransactionId(int transactionId)
{
    if (m_transactionId == transactionId)
        return;

    m_transactionId = transactionId;
    emit transactionIdChanged();
}

bool AbstractTransactionItemModel::isExistingTransaction() const
{
    return m_transactionId > 0;
}
