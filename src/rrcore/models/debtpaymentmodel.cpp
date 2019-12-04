#include "debtpaymentmodel.h"

#include <QDateTime>

DebtPaymentModel::DebtPaymentModel(QObject *parent) :
    AbstractVisualListModel(parent),
    m_transactionId(-1),
    m_dirty(false),
    m_totalAmount(0.0)
{

}

bool DebtPaymentModel::isDirty() const
{
    return m_dirty;
}

int DebtPaymentModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_records.count() + m_debtPayments.count();
}

QVariant DebtPaymentModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case AmountPaidRole:
        if (isExistingRecord(index.row()))
            return m_records.at(index.row()).toMap().value("amount_paid").toDouble();
        else
            return m_debtPayments.at(index.row() - m_records.count()).amount;
        break;
    case BalanceRole:
        if (isExistingRecord(index.row()))
            return m_records.at(index.row()).toMap().value("balance").toDouble();
        break;
    case CurrencyRole:
        if (isExistingRecord(index.row()))
            return m_records.at(index.row()).toMap().value("currency").toString();
        break;
    case DueDateRole:
        if (isExistingRecord(index.row()))
            return m_records.at(index.row()).toMap().value("due_date").toDateTime();
        break;
    case NoteRole:
        if (isExistingRecord(index.row()))
            return m_records.at(index.row()).toMap().value("note").toString();
        else
            return m_debtPayments.at(index.row() - m_records.count()).note;
        break;
    case UncommittedRole:
        return false;
        break;
    case ArchivedRole:
        if (isExistingRecord(index.row()))
            return m_records.at(index.row()).toMap().value("archived").toBool();
        break;
    case CreatedRole:
        if (isExistingRecord(index.row()))
            return m_records.at(index.row()).toMap().value("created").toDateTime();
        else
            return QDateTime::currentDateTime();
        break;
    }

    return QVariant();
}

QHash<int, QByteArray> DebtPaymentModel::roleNames() const
{
    return {
        { AmountPaidRole, "amount_paid" },
        { BalanceRole, "balance" },
        { CurrencyRole, "currency" },
        { DueDateRole, "due_date" },
        { NoteRole, "note" },
        { UncommittedRole, "uncommitted" },
        { ArchivedRole, "archived" },
        { CreatedRole, "created" }
    };
}

int DebtPaymentModel::transactionId() const
{
    return m_transactionId;
}

void DebtPaymentModel::setTransactionId(int transactionId)
{
    m_transactionId = transactionId;
}

void DebtPaymentModel::setTotalAmount(double totalAmount)
{
    if (totalAmount < 0.0)
        return;

    m_totalAmount = totalAmount;
}

void DebtPaymentModel::setPaymentRecords(const QVariantList &records)
{
    m_records = records;
}

void DebtPaymentModel::addPayment(DebtPayment debtPayment)
{
    if (m_debtPayments.contains(debtPayment))
        return;

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_debtPayments.append(debtPayment);
    endInsertRows();

    setDirty(m_debtPayments.count() != 0);
}

void DebtPaymentModel::updatePayment(DebtPayment debtPayment)
{
    const int row = m_debtPayments.indexOf(debtPayment);
    emit dataChanged(index(row), index(row));

    setDirty(m_debtPayments.count() != 0);
}

void DebtPaymentModel::removePayment(DebtPayment debtPayment)
{
    if (!m_debtPayments.contains(debtPayment))
        return;

    const int row = m_debtPayments.indexOf(debtPayment);
    beginRemoveRows(QModelIndex(), row, row);
    m_debtPayments.removeAll(debtPayment);
    endRemoveRows();

    setDirty(m_debtPayments.count() != 0);
}

void DebtPaymentModel::clearPayments()
{
    beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
    m_debtPayments.clear();
    endRemoveRows();

    setDirty(m_debtPayments.count() != 0);
}

void DebtPaymentModel::tryQuery()
{

}

void DebtPaymentModel::processResult(const QueryResult result)
{
    Q_UNUSED(result)
}

void DebtPaymentModel::setDirty(bool dirty)
{
    if (m_dirty == dirty)
        return;

    m_dirty = dirty;
    emit dirtyChanged();
}

bool DebtPaymentModel::isExistingRecord(int row) const
{
    return row >= 0 && row < m_records.count();
}
