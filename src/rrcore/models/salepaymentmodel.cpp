#include "salepaymentmodel.h"

#include "utility/saleutils.h"

SalePaymentModel::SalePaymentModel(QObject *parent) :
    AbstractVisualListModel(parent),
    m_cashPaymentCount(0),
    m_cardPaymentCount(0)
{
    qRegisterMetaType<SalePaymentModel*>();
}

SalePaymentModel::~SalePaymentModel()
{

}

int SalePaymentModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_salePayments.count();
}

QVariant SalePaymentModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case AmountRole:
        return m_salePayments.at(index.row()).amount;
    case MethodRole:
        return static_cast<int>(m_salePayments.at(index.row()).method);
    case NoteRole:
        return m_salePayments.at(index.row()).note;
    }

    return QVariant();
}

QHash<int, QByteArray> SalePaymentModel::roleNames() const
{
    return {
        { AmountRole, "amount" },
        { MethodRole, "method" },
        { NoteRole, "note" }
    };
}

void SalePaymentModel::addPayment(SalePayment payment)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_salePayments.append(payment);
    calculatePaymentCount();
    endInsertRows();
}

void SalePaymentModel::removePayment(int index)
{
    if (index < 0 || index >= m_salePayments.count())
        return;

    beginRemoveRows(QModelIndex(), index, index);
    m_salePayments.removeAt(index);
    calculatePaymentCount();
    endRemoveRows();
}

void SalePaymentModel::clearPayments()
{
    beginResetModel();
    m_salePayments.clear();
    calculatePaymentCount();
    endResetModel();
}

int SalePaymentModel::cashPaymentCount() const
{
    return m_cashPaymentCount;
}

int SalePaymentModel::cardPaymentCount() const
{
    return m_cardPaymentCount;
}

void SalePaymentModel::calculatePaymentCount()
{
    const int oldCashPaymentCount = m_cashPaymentCount;
    const int oldCardPaymentCount = m_cardPaymentCount;

    m_cashPaymentCount = 0;
    m_cardPaymentCount = 0;

    for (const SalePayment &payment : m_salePayments) {
        if (payment.method == SalePayment::PaymentMethod::Cash)
            m_cashPaymentCount++;
        else
            m_cardPaymentCount++;
    }

    if (m_cashPaymentCount != oldCashPaymentCount)
        emit cashPaymentCountChanged();
    if (m_cardPaymentCount != oldCardPaymentCount)
        emit cardPaymentCountChanged();
}

void SalePaymentModel::tryQuery()
{

}

void SalePaymentModel::processResult(const QueryResult result)
{
    Q_UNUSED(result)
}
