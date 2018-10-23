#include "abstracttransactionitemmodel.h"

AbstractTransactionItemModel::AbstractTransactionItemModel(QObject *parent) :
    AbstractVisualListModel(parent),
    m_transactionId(-1)
{
    connect(this, &AbstractTransactionItemModel::transactionIdChanged, &AbstractTransactionItemModel::tryQuery);
}

AbstractTransactionItemModel::AbstractTransactionItemModel(DatabaseThread &thread) :
    AbstractVisualListModel(thread),
    m_transactionId(-1)
{
    connect(this, &AbstractTransactionItemModel::transactionIdChanged, &AbstractTransactionItemModel::tryQuery);
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
