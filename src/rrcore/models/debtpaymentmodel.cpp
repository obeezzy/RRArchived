#include "debtpaymentmodel.h"

#include <QDateTime>

DebtPaymentModel::DebtPaymentModel(QObject *parent) :
    AbstractVisualListModel(parent),
    m_transactionId(-1)
{

}

int DebtPaymentModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_records.count();
}

QVariant DebtPaymentModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case AmountPaidRole:
        return m_records.at(index.row()).toMap().value("amount_paid").toDouble();
        break;
    case BalanceRole:
        return m_records.at(index.row()).toMap().value("balance").toDouble();
        break;
    case CurrencyRole:
        return m_records.at(index.row()).toMap().value("currency").toString();
        break;
    case DueDateRole:
        return m_records.at(index.row()).toMap().value("due_date").toDateTime();
        break;
    case NoteRole:
        return m_records.at(index.row()).toMap().value("note").toString();
        break;
    case ArchivedRole:
        return m_records.at(index.row()).toMap().value("archived").toBool();
        break;
    case CreatedRole:
        return m_records.at(index.row()).toMap().value("created").toDateTime();
        break;
    }

    return QVariant();
}

QHash<int, QByteArray> DebtPaymentModel::roleNames() const
{
    QHash<int, QByteArray> roles(AbstractVisualListModel::roleNames());
    roles.insert(AmountPaidRole, "amount_paid");
    roles.insert(BalanceRole, "balance");
    roles.insert(CurrencyRole, "currency");
    roles.insert(DueDateRole, "due_date");
    roles.insert(NoteRole, "note");
    roles.insert(ArchivedRole, "archived");
    roles.insert(CreatedRole, "created");

    return roles;
}

int DebtPaymentModel::transactionId() const
{
    return m_transactionId;
}

void DebtPaymentModel::setTransactionId(int transactionId)
{
    m_transactionId = transactionId;
}

void DebtPaymentModel::setPayments(const QVariantList &payments)
{
    m_records = payments;
}

bool DebtPaymentModel::addPayment()
{
    return false;
}

bool DebtPaymentModel::removePayment()
{
    return false;
}

void DebtPaymentModel::tryQuery()
{

}

void DebtPaymentModel::processResult(const QueryResult result)
{
    Q_UNUSED(result)
}
