#include "qmldebtpaymentmodel.h"
#include "database/databasethread.h"
#include "queryexecutors/debtor.h"

#include <QDateTime>

Q_LOGGING_CATEGORY(lcqmldebtpaymentmodel, "rrcore.models.qmldebtpaymentmodel");

QMLDebtPaymentModel::QMLDebtPaymentModel(QObject *parent) :
    QMLDebtPaymentModel(DatabaseThread::instance(), parent)
{}

QMLDebtPaymentModel::QMLDebtPaymentModel(DatabaseThread &thread, QObject *parent) :
    AbstractVisualListModel(thread, parent),
    m_debtTransactionId(-1),
    m_totalAmountPaid(0.0),
    m_lastPaymentId(0)
{
    connect(this, &QMLDebtPaymentModel::debtTransactionIdChanged,
            this, &QMLDebtPaymentModel::tryQuery);
}

int QMLDebtPaymentModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_payments.count();
}

QVariant QMLDebtPaymentModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case AmountOwedRole: {
        auto debtTransaction = m_debtTransactionRef.value<DebtTransaction *>();
        return debtTransaction->totalDebt;
    }
    case AmountPaidRole:
        return m_payments.at(index.row()).amountPaid;
    case BalanceRole:
        return m_payments.at(index.row()).balance;
    case MaxPayableAmount: {
        auto debtTransaction = m_debtTransactionRef.value<DebtTransaction *>();
        return qMax(0.0, debtTransaction->totalDebt - m_totalAmountPaid);
    }
    case CurrencyRole:
        return m_payments.at(index.row()).currency;
    case DueDateRole:
        return m_payments.at(index.row()).dueDateTime;
    case NoteRole:
        return m_payments.at(index.row()).note.note;
    case DirtyRole:
        return m_payments.at(index.row()).state == DebtPayment::State::Dirty;
    case FreshRole:
        return m_payments.at(index.row()).state == DebtPayment::State::Fresh;
    case ArchivedRole:
        return m_payments.at(index.row()).archived;
    case CreatedRole:
        return m_payments.at(index.row()).created;
    case LastEditedRole:
        return m_payments.at(index.row()).lastEdited;
    }

    return QVariant();
}

QHash<int, QByteArray> QMLDebtPaymentModel::roleNames() const
{
    return {
        { AmountOwedRole, "amount_owed" },
        { AmountPaidRole, "amount_paid" },
        { BalanceRole, "balance" },
        { MaxPayableAmount, "max_payable_amount" },
        { CurrencyRole, "currency" },
        { DueDateRole, "due_date" },
        { NoteRole, "note" },
        { DirtyRole, "dirty" },
        { FreshRole, "fresh" },
        { ArchivedRole, "archived" },
        { CreatedRole, "created" },
        { LastEditedRole, "last_edited" }
    };
}

bool QMLDebtPaymentModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    DebtPayment &payment = m_payments[index.row()];

    switch (role) {
    case AmountPaidRole:
        if (payment.state != DebtPayment::State::Clean) {
            payment.amountPaid = value.toDouble();
            updateRef(payment);
            emit dataChanged(index, index);
            calculateTotals();
        } else {
            qCWarning(lcqmldebtpaymentmodel) << "Can't update amount paid: row must be dirty or fresh.";
            return false;
        }
        break;
    case DueDateRole:
        payment.dueDateTime = value.toDateTime();
        updateRef(payment);
        emit dataChanged(index, index);
        break;
    default:
        qCWarning(lcqmldebtpaymentmodel) << "Role not mutable:" << roleNames().value(role);
        return false;
    }

    return true;
}

int QMLDebtPaymentModel::debtTransactionId() const
{
    return m_debtTransactionId;
}

void QMLDebtPaymentModel::setDebtTransactionId(int debtTransactionId)
{
    if (m_debtTransactionId == debtTransactionId)
        return;

    m_debtTransactionId = debtTransactionId;
    emit debtTransactionIdChanged();
}

QVariant QMLDebtPaymentModel::debtTransactionRef() const
{
    return m_debtTransactionRef;
}

void QMLDebtPaymentModel::setDebtTransactionRef(const QVariant &debtTransactionRef)
{
    if (m_debtTransactionRef == debtTransactionRef)
        return;

    m_debtTransactionRef = debtTransactionRef;
    emit debtTransactionRefChanged();
}

qreal QMLDebtPaymentModel::totalAmountPaid() const
{
    return m_totalAmountPaid;
}

void QMLDebtPaymentModel::setTotalAmountPaid(qreal totalAmountPaid)
{
    if (m_totalAmountPaid == totalAmountPaid)
        return;

    m_totalAmountPaid = totalAmountPaid;
    emit totalAmountPaidChanged();
}

void QMLDebtPaymentModel::addPayment(double amount,
                                     const QString &note)
{
    if (amount <= 0.0)
        return;

    auto debtTransaction = m_debtTransactionRef.value<DebtTransaction *>();
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    if (!m_payments.isEmpty()) {
        m_payments.append(DebtPayment{m_payments.last().balance, amount,
                                      debtTransaction->dueDateTime, Note{note}});
    } else {
        m_payments.append(DebtPayment{debtTransaction->totalDebt, amount,
                                      debtTransaction->dueDateTime, Note{note}});
    }
    updateRef(m_payments.last());
    calculateTotals();
    endInsertRows();
}

void QMLDebtPaymentModel::removePayment(int row)
{
    if (row >= 0 && row < rowCount() - 1)
        return;

    beginRemoveRows(QModelIndex(), row, row);
    auto payment = m_payments.takeAt(row);
    payment.state = DebtPayment::State::Trash;
    updateRef(payment);
    calculateTotals();
    endRemoveRows();
}

void QMLDebtPaymentModel::tryQuery()
{
    setBusy(true);
    emit execute(new DebtorQuery::ViewDebtPayments(m_debtTransactionId, this));
}

void QMLDebtPaymentModel::processResult(const QueryResult result)
{
    if (result.request().receiver() != this)
        return;

    setBusy(false);

    if (result.isSuccessful()) {
        beginResetModel();
        m_payments = DebtPaymentList{result.outcome().toMap().value("payments").toList()};
        calculateTotals();
        endResetModel();

        emit success(ViewDebtorPaymentsSuccess);
    } else {
        emit error(UnknownError);
    }
}

void QMLDebtPaymentModel::calculateTotals()
{
    qreal totalAmountPaid = 0.0;
    for (const auto &payment : m_payments)
        totalAmountPaid += payment.amountPaid;

    setTotalAmountPaid(totalAmountPaid);
}

void QMLDebtPaymentModel::updateRef(DebtPayment payment)
{
    auto debtTransaction = m_debtTransactionRef.value<DebtTransaction *>();
    payment.debtTransactionId = debtTransaction->id;

    if (payment.state == DebtPayment::State::Trash) {
        debtTransaction->debtPayments.removeAll(payment);
        debtTransaction->archivedDebtPayments.append(payment);
    } else if (debtTransaction->debtPayments.contains(payment)) {
        debtTransaction->debtPayments.replace(debtTransaction->debtPayments.indexOf(payment), payment);
    } else {
        debtTransaction->debtPayments.append(payment);
    }

    debtTransaction->lastEdited = QDateTime::currentDateTime();
    debtTransaction->state = DebtTransaction::State::Dirty;
}
