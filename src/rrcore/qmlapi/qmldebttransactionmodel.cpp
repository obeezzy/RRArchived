#include "qmldebttransactionmodel.h"
#include "database/databaseerror.h"
#include "database/databasethread.h"
#include "queryexecutors/debtor.h"
#include "utility/debtorutils.h"
#include <QDateTime>

Q_LOGGING_CATEGORY(lcqmldebttransactionmodel, "rrcore.models.qmldebttransactionmodel");

QMLDebtTransactionModel::QMLDebtTransactionModel(QObject *parent) :
    QMLDebtTransactionModel(DatabaseThread::instance(), parent)
{}

QMLDebtTransactionModel::QMLDebtTransactionModel(DatabaseThread &thread, QObject *parent) :
    AbstractVisualListModel(thread, parent)
{
    connect(this, &QMLDebtTransactionModel::debtorIdChanged,
            this, &QMLDebtTransactionModel::tryQuery);
}

int QMLDebtTransactionModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_debtor.transactions.count();
}

QVariant QMLDebtTransactionModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case TransactionIdRole:
        return m_debtor.transactions.at(index.row()).id;
    case RelatedTransactionRole:
        return m_debtor.transactions.at(index.row()).relatedTransaction.toString();
    case RelatedTransactionIdRole:
        return m_debtor.transactions.at(index.row()).relatedTransaction.id;
    case NoteRole:
        return m_debtor.transactions.at(index.row()).note.note;
    case DueDateRole:
        return m_debtor.transactions.at(index.row()).dueDateTime;
    case CreatedRole:
        return m_debtor.transactions.at(index.row()).timestamp.created;
    case RefRole:
        return QVariant::fromValue(&m_debtor.transactions.at(index.row()));
    case TotalDebtRole:
        return m_debtor.transactions.at(index.row()).totalDebt.toDouble();
    case DirtyRole:
        return m_debtor.transactions.at(index.row()).state == Utility::DebtTransaction::State::Dirty;
    }

    return QVariant();
}

QHash<int, QByteArray> QMLDebtTransactionModel::roleNames() const
{
    return {
        { TransactionIdRole, "transaction_id" },
        { RelatedTransactionRole, "related_transaction" },
        { RelatedTransactionIdRole, "related_transaction_id" },
        { DueDateRole, "due_date" },
        { DirtyRole, "dirty" },
        { NoteRole, "note" },
        { CreatedRole, "created" },
        { RefRole, "ref" },
        { TotalDebtRole, "total_debt" }
    };
}

bool QMLDebtTransactionModel::setData(const QModelIndex &index,
                                      const QVariant &value,
                                      int role)
{
    if (!index.isValid())
        return false;

    Utility::DebtTransaction &debtTransaction = m_debtor.transactions[index.row()];
    switch (role) {
    case DueDateRole:
        if (value.toDateTime().isValid() && (debtTransaction.dueDateTime != value.toDateTime())) {
            debtTransaction.dueDateTime = value.toDateTime();
            m_dirty = true;
            emit dataChanged(index, index);
        } else if (debtTransaction.dueDateTime == value.toDateTime()) {
            return false;
        } else {
            qCWarning(lcqmldebttransactionmodel) << "Can't update due date/time: date/time invalid.";
            return false;
        }
        break;
    case NoteRole:
        if (debtTransaction.note.note != value.toString()) {
            debtTransaction.note.note = value.toString();
            m_dirty = true;
            emit dataChanged(index, index);
        }
        break;
    default:
        qCWarning(lcqmldebttransactionmodel) << "Role not mutable:" << roleNames().value(role);
        return false;
    }

    return true;
}

int QMLDebtTransactionModel::debtorId() const
{
    return m_debtor.id;
}

void QMLDebtTransactionModel::setDebtorId(int debtorId)
{
    if (m_debtor.id == debtorId)
        return;

    m_debtor.id = debtorId;
    emit debtorIdChanged();
}

int QMLDebtTransactionModel::clientId() const
{
    return m_debtor.client.id;
}

void QMLDebtTransactionModel::setClientId(int clientId)
{
    if (m_debtor.client.id == clientId)
        return;

    m_debtor.client.id = clientId;
    emit clientIdChanged();
}

QUrl QMLDebtTransactionModel::imageUrl() const
{
    return m_debtor.client.imageUrl;
}

void QMLDebtTransactionModel::setImageUrl(const QUrl &imageUrl)
{
    if (m_debtor.client.imageUrl == imageUrl)
        return;

    m_debtor.client.imageUrl = imageUrl;
    emit imageUrlChanged();
}

QString QMLDebtTransactionModel::firstName() const
{
    return m_debtor.client.firstName;
}

void QMLDebtTransactionModel::setFirstName(const QString &firstName)
{
    if (m_debtor.client.firstName == firstName)
        return;

    m_debtor.client.firstName = firstName;
    emit firstNameChanged();
}

QString QMLDebtTransactionModel::lastName() const
{
    return m_debtor.client.lastName;
}

void QMLDebtTransactionModel::setLastName(const QString &lastName)
{
    if (m_debtor.client.lastName == lastName)
        return;

    m_debtor.client.lastName = lastName;
    emit lastNameChanged();
}

QString QMLDebtTransactionModel::preferredName() const
{
    return m_debtor.client.preferredName;
}

void QMLDebtTransactionModel::setPreferredName(const QString &preferredName)
{
    if (m_debtor.client.preferredName == preferredName)
        return;

    m_debtor.client.preferredName = preferredName;
    emit preferredNameChanged();
}

QString QMLDebtTransactionModel::phoneNumber() const
{
    return m_debtor.client.phoneNumber;
}

void QMLDebtTransactionModel::setPhoneNumber(const QString &phoneNumber)
{
    if (m_debtor.client.phoneNumber == phoneNumber)
        return;

    m_debtor.client.phoneNumber = phoneNumber;
    emit phoneNumberChanged();
}

QString QMLDebtTransactionModel::note() const
{
    return m_debtor.note.note;
}

void QMLDebtTransactionModel::setNote(const QString &note)
{
    if (m_debtor.note.note == note)
        return;

    m_debtor.note.note = note;
    emit noteChanged();
}

void QMLDebtTransactionModel::addDebt(double totalDebt,
                                      const QDateTime &dueDateTime,
                                      const QString &note)
{
    if (totalDebt <= 0.0 || !dueDateTime.isValid())
        return;

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_debtor.transactions.append(Utility::DebtTransaction{
                                         totalDebt,
                                         dueDateTime,
                                         Utility::Note{ note }
                                     });
    m_dirty = true;
    endInsertRows();
}

void QMLDebtTransactionModel::removeDebt(int row)
{
    if (!index(row).isValid()) {
        qCWarning(lcqmldebttransactionmodel) << "Row out of range value in:" << Q_FUNC_INFO;
        return;
    }

    beginRemoveRows(QModelIndex(), row, row);
    m_debtor.transactions.takeAt(row).clear();
    m_dirty = true;
    endRemoveRows();
}

void QMLDebtTransactionModel::tryQuery()
{
    if (m_debtor.id <= 0)
        return;

    setBusy(true);
    emit execute(new DebtorQuery::ViewDebtTransactions(m_debtor, this));
}

void QMLDebtTransactionModel::processResult(const QueryResult result)
{
    if (result.request().receiver() != this)
        return;

    setBusy(false);

    if (result.isSuccessful()) {
        if (result.request().command() == DebtorQuery::ViewDebtTransactions::COMMAND) {
            beginResetModel();

            clearAll();
            m_debtor.transactions = Utility::DebtTransactionList{ result.outcome().toMap().value("transactions").toList() };
            const auto &debtor = Utility::Debtor{ result.outcome().toMap() };
            setClientId(debtor.client.id);
            setPreferredName(debtor.client.preferredName);
            setPhoneNumber(debtor.client.phoneNumber);
            setNote(debtor.note.note);

            endResetModel();

            emit success(ModelResult{ ViewDebtorTransactionsSuccess });
        } else if (result.request().command() == DebtorQuery::AddDebtor::COMMAND) {
            clearAll();
            emit success(ModelResult{ AddDebtorSuccess });
        } else if (result.request().command() == DebtorQuery::AddDebtor::UNDO_COMMAND) {
            clearAll();
            emit success(ModelResult{ UndoAddDebtorSuccess });
        } else if (result.request().command() == DebtorQuery::UpdateDebtor::COMMAND) {
            clearAll();
            emit success(ModelResult{ UpdateDebtorSuccess });
        } else {
            clearAll();
            emit success();
        }
    } else {
        switch (result.errorCode()) {
        case int(DatabaseError::QueryErrorCode::DuplicateEntryFailure):
            emit error(ModelResult{ DuplicateEntryError });
            break;
        case int(DatabaseError::QueryErrorCode::AmountOverpaid):
            emit error(ModelResult{ AmountOverpaidError });
            break;
        case int(DatabaseError::QueryErrorCode::InvalidDueDate):
            emit error(ModelResult{ InvalidDueDateError });
            break;
        default:
            emit error();
            break;
        }
    }
}

void QMLDebtTransactionModel::clearAll()
{
    setPreferredName(QString());
    setFirstName(QString());
    setLastName(QString());
    setPhoneNumber(QString());
    m_debtor.transactions.clear();
    m_dirty = false;
}

bool QMLDebtTransactionModel::isExistingDebtor() const
{
    return m_debtor.id > 0;
}

bool QMLDebtTransactionModel::paymentsDirty() const
{
    for (const auto &transaction : m_debtor.transactions) {
        for (const auto &payment : transaction.payments) {
            Q_UNUSED(payment)
            return true;
        }
    }

    return false;
}

bool QMLDebtTransactionModel::paymentsDueDateValid() const
{
    for (const auto &transaction : m_debtor.transactions) {
        for (const auto &payment : transaction.payments) {
            if (payment.dueDateTime <= QDateTime::currentDateTime())
                return false;
        }
    }

    return true;
}

bool QMLDebtTransactionModel::submit()
{
    if (m_debtor.client.phoneNumber.trimmed().isEmpty()) {
        emit error(ModelResult{ NoPhoneNumberError });
        return false;
    } else if (m_debtor.client.preferredName.trimmed().isEmpty()) {
        emit error(ModelResult{ NoPreferredNameError });
        return false;
    } else if (!m_dirty && !paymentsDirty()) {
        emit error(ModelResult{ DataUnchangedError });
        return false;
    } else if (!paymentsDueDateValid()) {
        emit error(ModelResult{ InvalidDueDateError });
        return false;
    } else {
        setBusy(true);

        if (isExistingDebtor())
            emit execute(new DebtorQuery::UpdateDebtor(m_debtor, this));
        else
            emit execute(new DebtorQuery::AddDebtor(m_debtor, this));
    }

    return true;
}
