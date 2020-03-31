#include "qmldebtpaymentmodel.h"
#include "database/databasethread.h"
#include "queryexecutors/debtor.h"
#include "utility/debtorutils.h"
#include <QDateTime>

Q_LOGGING_CATEGORY(lcqmldebtpaymentmodel, "rrcore.models.qmldebtpaymentmodel");

QMLDebtPaymentModel::QMLDebtPaymentModel(QObject *parent) :
    QMLDebtPaymentModel(DatabaseThread::instance(), parent)
{}

QMLDebtPaymentModel::QMLDebtPaymentModel(DatabaseThread &thread, QObject *parent) :
    AbstractVisualListModel(thread, parent)
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
    case DebtRole:
        return m_payments.at(index.row()).monies.debt.toDouble();
    case AmountPaidRole:
        return m_payments.at(index.row()).monies.amountPaid.toDouble();
    case BalanceRole:
        return m_payments.at(index.row()).monies.balance.toDouble();
    case MaxPayableAmount: {
        if (!m_debtTransactionRef.canConvert<Utility::DebtTransaction *>()) {
            qCWarning(lcqmldebtpaymentmodel) << "Invalid debt transaction. Can't display max payable amount.";
            return QVariant();
        }
        auto debtTransaction = m_debtTransactionRef.value<Utility::DebtTransaction *>();
        return qMax(Utility::Money(0.0), debtTransaction->totalDebt - m_totalAmountPaid).toDouble();
    }
    case CurrencyRole:
        return m_payments.at(index.row()).monies.debt.currency().isoCode();
    case DueDateRole:
        return m_payments.at(index.row()).dueDateTime;
    case NoteRole:
        return m_payments.at(index.row()).note.note;
    case DirtyRole:
        return m_payments.at(index.row()).state == Utility::DebtPayment::State::Dirty;
    case FreshRole:
        return m_payments.at(index.row()).state == Utility::DebtPayment::State::Fresh;
    case ArchivedRole:
        return m_payments.at(index.row()).flags.testFlag(Utility::RecordGroup::Archived);
    case CreatedRole:
        return m_payments.at(index.row()).timestamp.created.isNull() ? QDateTime::currentDateTime()
                                                                     : m_payments.at(index.row()).timestamp.created;
    case LastEditedRole:
        return m_payments.at(index.row()).timestamp.lastEdited;
    }

    return QVariant();
}

QHash<int, QByteArray> QMLDebtPaymentModel::roleNames() const
{
    return {
        { DebtRole, "debt" },
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

    Utility::DebtPayment &payment{ m_payments[index.row()] };

    switch (role) {
    case AmountPaidRole:
        if (!value.canConvert<double>())
            qCWarning(lcqmldebtpaymentmodel) << "Invalid 'amount_paid' value.";
        if (payment.state != Utility::DebtPayment::State::Clean) {
            payment.monies.amountPaid = Utility::Money{ value.toDouble() };
            updateRef(payment);
            emit dataChanged(index, index);
            calculateTotalAmountPaid();
        } else {
            qCWarning(lcqmldebtpaymentmodel) << "Can't update amount paid: row must be dirty or fresh.";
            return false;
        }
        break;
    case DueDateRole:
        if (!value.canConvert<QDateTime>())
            qCWarning(lcqmldebtpaymentmodel) << "Invalid 'due_date' value.";
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

double QMLDebtPaymentModel::totalAmountPaid() const
{
    return m_totalAmountPaid.toDouble();
}

void QMLDebtPaymentModel::setTotalAmountPaid(double totalAmountPaid)
{
    if (m_totalAmountPaid == Utility::Money(totalAmountPaid))
        return;

    m_totalAmountPaid = Utility::Money(totalAmountPaid);
    emit totalAmountPaidChanged();
}

void QMLDebtPaymentModel::addPayment(double amount,
                                     const QString &note)
{
    if (amount <= 0.0)
        return;

    if (!m_debtTransactionRef.canConvert<Utility::DebtTransaction *>()) {
        qCWarning(lcqmldebtpaymentmodel) << "Invalid debt transaction. Can't add payment.";
        return;
    }
    auto debtTransaction = m_debtTransactionRef.value<Utility::DebtTransaction *>();

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    if (!m_payments.isEmpty()) {
        const double balance = (m_payments.last().monies.balance - Utility::Money(amount)).toDouble();
        m_payments.append(Utility::DebtPayment{ Utility::DebtMonies {
                                                    QVariantMap {
                                                        { "debt", m_payments.last().monies.balance.toDouble() },
                                                        { "amount_paid", amount },
                                                        { "balance", balance }
                                                    }
                                               },
                                               debtTransaction->dueDateTime,
                                               Utility::Note{ note }});
    } else {
        const double balance = (debtTransaction->totalDebt - Utility::Money(amount)).toDouble();
        m_payments.append(Utility::DebtPayment{ Utility::DebtMonies {
                                                    QVariantMap {
                                                        { "debt", debtTransaction->totalDebt.toDouble() },
                                                        { "amount_paid", amount },
                                                        { "balance", balance }
                                                    }
                                               },
                                               debtTransaction->dueDateTime,
                                               Utility::Note{ note }});
    }
    m_payments.last().timestamp = Utility::RecordTimestamp {
            QDateTime::currentDateTime(),
            QDateTime::currentDateTime()
    };
    updateRef(m_payments.last());
    calculateTotalAmountPaid();
    endInsertRows();
}

void QMLDebtPaymentModel::removePayment(int row)
{
    if (row >= 0 && row < rowCount() - 1)
        return;

    beginRemoveRows(QModelIndex(), row, row);
    auto payment = m_payments.takeAt(row);
    payment.state = Utility::DebtPayment::State::Trash;
    updateRef(payment);
    calculateTotalAmountPaid();
    endRemoveRows();
}

void QMLDebtPaymentModel::tryQuery()
{
    setBusy(true);
    emit execute(new DebtorQuery::ViewDebtPayments(m_debtTransactionId,
                                                   this));
}

bool QMLDebtPaymentModel::canProcessResult(const QueryResult &result) const
{
    Q_UNUSED(result)
    return true;
}

void QMLDebtPaymentModel::processResult(const QueryResult &result)
{
    if (result.request().receiver() != this)
        return;

    setBusy(false);

    if (result.isSuccessful()) {
        beginResetModel();
        m_payments = Utility::DebtPaymentList{ result.outcome().toMap().value("payments").toList() };
        calculateTotalAmountPaid();
        endResetModel();

        emit success();
    } else {
        emit error();
    }
}

void QMLDebtPaymentModel::calculateTotalAmountPaid()
{
    Utility::Money totalAmountPaid;
    for (const auto &payment : m_payments)
        totalAmountPaid += payment.monies.amountPaid;

    setTotalAmountPaid(totalAmountPaid.toDouble());
}

void QMLDebtPaymentModel::updateRef(Utility::DebtPayment payment)
{
    if (!m_debtTransactionRef.canConvert<Utility::DebtTransaction *>()) {
        qCWarning(lcqmldebtpaymentmodel) << "Invalid debt transaction. Can't update ref.";
        return;
    }

    auto debtTransaction = m_debtTransactionRef.value<Utility::DebtTransaction *>();
    payment.debtTransactionId = debtTransaction->id;

    if (payment.state == Utility::DebtPayment::State::Trash) {
        debtTransaction->payments.removeAll(payment);
        debtTransaction->archivedPayments.append(payment);
    } else if (debtTransaction->payments.contains(payment)) {
        debtTransaction->payments.replace(debtTransaction->payments.indexOf(payment), payment);
    } else {
        debtTransaction->payments.append(payment);
    }

    debtTransaction->timestamp.lastEdited = QDateTime::currentDateTime();
    debtTransaction->state = Utility::DebtTransaction::State::Dirty;
}
