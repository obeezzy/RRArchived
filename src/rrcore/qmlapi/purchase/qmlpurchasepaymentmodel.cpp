#include "qmlpurchasepaymentmodel.h"

QMLPurchasePaymentModel::QMLPurchasePaymentModel(QObject* parent)
    : AbstractVisualListModel(parent)
{}

int QMLPurchasePaymentModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;

    return m_payments.count();
}

QVariant QMLPurchasePaymentModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
        case AmountRole:
            return m_payments.at(index.row()).amount.toDouble();
        case MethodRole:
            return m_payments.at(index.row()).method.toString();
        case NoteRole:
            return m_payments.at(index.row()).note.note;
    }

    return QVariant();
}

QHash<int, QByteArray> QMLPurchasePaymentModel::roleNames() const
{
    return {{AmountRole, "amount"}, {MethodRole, "method"}, {NoteRole, "note"}};
}

double QMLPurchasePaymentModel::totalAmount() const
{
    return m_totalAmount.toDouble();
}

void QMLPurchasePaymentModel::addPayment(
    const Utility::Purchase::PurchasePayment& payment)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_payments.append(payment);
    endInsertRows();
}

void QMLPurchasePaymentModel::removePayment(int index)
{
    if (index < 0 || index >= m_payments.count())
        return;

    beginRemoveRows(QModelIndex(), index, index);
    m_payments.removeAt(index);
    endRemoveRows();
}

void QMLPurchasePaymentModel::clearPayments()
{
    beginResetModel();
    m_payments.clear();
    endResetModel();
}

int QMLPurchasePaymentModel::cashPaymentCount() const
{
    return m_payments.count(Utility::PaymentMethod::Cash);
}

int QMLPurchasePaymentModel::cardPaymentCount() const
{
    return m_payments.count(Utility::PaymentMethod::DebitCard) +
           m_payments.count(Utility::PaymentMethod::CreditCard);
}

void QMLPurchasePaymentModel::calculateTotalAmount()
{
    setTotalAmount(m_payments.total().toDouble());
}

void QMLPurchasePaymentModel::tryQuery() {}

bool QMLPurchasePaymentModel::canProcessResult(const QueryResult& result) const
{
    Q_UNUSED(result)
    return true;
}

void QMLPurchasePaymentModel::processResult(const QueryResult& result)
{
    Q_UNUSED(result)
}

void QMLPurchasePaymentModel::setTotalAmount(double totalAmount)
{
    if (m_totalAmount == Utility::Money(totalAmount))
        return;

    m_totalAmount = Utility::Money(totalAmount);
    emit totalAmountChanged();
}
