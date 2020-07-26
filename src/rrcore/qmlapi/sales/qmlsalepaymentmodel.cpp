#include "qmlsalepaymentmodel.h"
#include "database/databasethread.h"
#include "utility/sales/salepayment.h"

QMLSalePaymentModel::QMLSalePaymentModel(QObject *parent) :
    QMLSalePaymentModel(DatabaseThread::instance(), parent)
{}

QMLSalePaymentModel::QMLSalePaymentModel(DatabaseThread &thread,
                                         QObject *parent) :
    AbstractVisualListModel(thread, parent)
{}

int QMLSalePaymentModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_payments.count();
}

QVariant QMLSalePaymentModel::data(const QModelIndex &index, int role) const
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

QHash<int, QByteArray> QMLSalePaymentModel::roleNames() const
{
    return {
        { AmountRole, "amount" },
        { MethodRole, "method" },
        { NoteRole, "note" }
    };
}

void QMLSalePaymentModel::addPayment(Utility::Sales::SalePayment payment)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_payments.append(payment);
    endInsertRows();
}

void QMLSalePaymentModel::removePayment(int index)
{
    if (index < 0 || index >= m_payments.count())
        return;

    beginRemoveRows(QModelIndex(), index, index);
    m_payments.removeAt(index);
    endRemoveRows();
}

void QMLSalePaymentModel::clearPayments()
{
    beginResetModel();
    m_payments.clear();
    endResetModel();
}

int QMLSalePaymentModel::cashPaymentCount() const
{
    return m_payments.count(Utility::PaymentMethod::Cash);
}

int QMLSalePaymentModel::cardPaymentCount() const
{
    return m_payments.count(Utility::PaymentMethod::DebitCard)
            + m_payments.count(Utility::PaymentMethod::CreditCard);
}

void QMLSalePaymentModel::calculateTotalAmount()
{
    setTotalAmount(m_payments.total().toDouble());
}

void QMLSalePaymentModel::tryQuery()
{

}

bool QMLSalePaymentModel::canProcessResult(const QueryResult &result) const
{
    Q_UNUSED(result)
    return true;
}

void QMLSalePaymentModel::processResult(const QueryResult &result)
{
    Q_UNUSED(result)
}

void QMLSalePaymentModel::setTotalAmount(double totalAmount)
{
    if (m_totalAmount == Utility::Money(totalAmount))
        return;

    m_totalAmount = Utility::Money(totalAmount);
    emit totalAmountChanged();
}
