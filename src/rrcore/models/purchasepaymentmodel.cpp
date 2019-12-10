#include "purchasepaymentmodel.h"

PurchasePaymentModel::PurchasePaymentModel(QObject *parent) :
    AbstractVisualListModel(parent),
    m_cashPaymentCount(0),
    m_cardPaymentCount(0)
{
}

PurchasePaymentModel::~PurchasePaymentModel()
{

}

int PurchasePaymentModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_purchasePayments.count();
}

QVariant PurchasePaymentModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case AmountRole:
        return m_purchasePayments.at(index.row()).amount;
    case MethodRole:
        return static_cast<int>(m_purchasePayments.at(index.row()).method);
    case NoteRole:
        return m_purchasePayments.at(index.row()).note;
    }

    return QVariant();
}

QHash<int, QByteArray> PurchasePaymentModel::roleNames() const
{
    return {
        { AmountRole, "amount" },
        { MethodRole, "method" },
        { NoteRole, "note" }
    };
}

void PurchasePaymentModel::addPayment(const PurchasePayment &payment)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_purchasePayments.append(payment);
    calculatePaymentCount();
    endInsertRows();
}

void PurchasePaymentModel::removePayment(int index)
{
    if (index < 0 || index >= m_purchasePayments.count())
        return;

    beginRemoveRows(QModelIndex(), index, index);
    m_purchasePayments.removeAt(index);
    calculatePaymentCount();
    endRemoveRows();
}

void PurchasePaymentModel::clearPayments()
{
    beginResetModel();
    m_purchasePayments.clear();
    calculatePaymentCount();
    endResetModel();
}

int PurchasePaymentModel::cashPaymentCount() const
{
    return m_cashPaymentCount;
}

int PurchasePaymentModel::cardPaymentCount() const
{
    return m_cardPaymentCount;
}

void PurchasePaymentModel::calculatePaymentCount()
{
    const int oldCashPaymentCount = m_cashPaymentCount;
    const int oldCardPaymentCount = m_cardPaymentCount;

    m_cashPaymentCount = 0;
    m_cardPaymentCount = 0;

    for (const PurchasePayment &payment : m_purchasePayments) {
        if (payment.method == PurchasePayment::PaymentMethod::Cash)
            m_cashPaymentCount++;
        else
            m_cardPaymentCount++;
    }

    if (m_cashPaymentCount != oldCashPaymentCount)
        emit cashPaymentCountChanged();
    if (m_cardPaymentCount != oldCardPaymentCount)
        emit cardPaymentCountChanged();
}

void PurchasePaymentModel::tryQuery()
{

}

void PurchasePaymentModel::processResult(const QueryResult result)
{
    Q_UNUSED(result)
}
