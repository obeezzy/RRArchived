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
    AbstractVisualListModel(thread, parent),
    m_debtorId(-1),
    m_clientId(-1),
    m_dirty(false)
{
    connect(this, &QMLDebtTransactionModel::debtorIdChanged,
            this, &QMLDebtTransactionModel::tryQuery);
}

int QMLDebtTransactionModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_transactions.count();
}

QVariant QMLDebtTransactionModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case TransactionIdRole:
        return m_transactions.at(index.row()).id;
    case RelatedTransactionRole:
        return tableNameNormalized(m_transactions.at(index.row()).relatedTransaction.tableName);
    case RelatedTransactionIdRole:
        return m_transactions.at(index.row()).relatedTransaction.id;
    case NoteRole:
        return m_transactions.at(index.row()).note.note;
    case DueDateRole:
        return m_transactions.at(index.row()).dueDateTime;
    case CreatedRole:
        return m_transactions.at(index.row()).created;
    case RefRole:
        return QVariant::fromValue(&m_transactions.at(index.row()));
    case TotalDebtRole:
        return m_transactions.at(index.row()).totalDebt;
    case DirtyRole:
        return m_transactions.at(index.row()).state == DebtTransaction::State::Dirty;
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

bool QMLDebtTransactionModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    DebtTransaction &transaction = m_transactions[index.row()];
    switch (role) {
    case DueDateRole:
        if (value.toDateTime().isValid() && (transaction.dueDateTime != value.toDateTime())) {
            transaction.dueDateTime = value.toDateTime();
            m_dirty = true;
            emit dataChanged(index, index);
        } else if (transaction.dueDateTime == value.toDateTime()) {
            return false;
        } else {
            qCWarning(lcqmldebttransactionmodel) << "Can't update due date/time: date/time invalid.";
            return false;
        }
        break;
    case NoteRole:
        if (transaction.note.note != value.toString()) {
            transaction.note.note = value.toString();
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
    return m_debtorId;
}

void QMLDebtTransactionModel::setDebtorId(int debtorId)
{
    if (m_debtorId == debtorId)
        return;

    m_debtorId = debtorId;
    emit debtorIdChanged();
}

int QMLDebtTransactionModel::clientId() const
{
    return m_clientId;
}

void QMLDebtTransactionModel::setClientId(int clientId)
{
    if (m_clientId == clientId)
        return;

    m_clientId = clientId;
    emit clientIdChanged();
}

QString QMLDebtTransactionModel::tableNameNormalized(const QString &tableName) const
{
    if (tableName == QStringLiteral("sale_transaction"))
        return tr("Sales");

    return tableName;
}

QUrl QMLDebtTransactionModel::imageUrl() const
{
    return m_imageUrl;
}

void QMLDebtTransactionModel::setImageUrl(const QUrl &imageUrl)
{
    if (m_imageUrl == imageUrl)
        return;

    m_imageUrl = imageUrl;
    emit imageUrlChanged();
}

QString QMLDebtTransactionModel::firstName() const
{
    return m_firstName;
}

void QMLDebtTransactionModel::setFirstName(const QString &firstName)
{
    if (m_firstName == firstName)
        return;

    m_firstName = firstName;
    emit firstNameChanged();
}

QString QMLDebtTransactionModel::lastName() const
{
    return m_lastName;
}

void QMLDebtTransactionModel::setLastName(const QString &lastName)
{
    if (m_lastName == lastName)
        return;

    m_lastName = lastName;
    emit lastNameChanged();
}

QString QMLDebtTransactionModel::preferredName() const
{
    return m_preferredName;
}

void QMLDebtTransactionModel::setPreferredName(const QString &preferredName)
{
    if (m_preferredName == preferredName)
        return;

    m_preferredName = preferredName;
    emit preferredNameChanged();
}

QString QMLDebtTransactionModel::primaryPhoneNumber() const
{
    return m_primaryPhoneNumber;
}

void QMLDebtTransactionModel::setPrimaryPhoneNumber(const QString &primaryPhoneNumber)
{
    if (m_primaryPhoneNumber == primaryPhoneNumber)
        return;

    m_primaryPhoneNumber = primaryPhoneNumber;
    emit primaryPhoneNumberChanged();
}

QStringList QMLDebtTransactionModel::alternatePhoneNumberModel() const
{
    return m_alternatePhoneNumberModel;
}

QStringList QMLDebtTransactionModel::addressModel() const
{
    return m_addressModel;
}

QStringList QMLDebtTransactionModel::emailAddressModel() const
{
    return m_emailAddressModel;
}

QString QMLDebtTransactionModel::note() const
{
    return m_note.note;
}

void QMLDebtTransactionModel::setNote(const QString &note)
{
    if (m_note.note == note)
        return;

    m_note.note = note;
    emit noteChanged();
}

void QMLDebtTransactionModel::addAlternatePhoneNumber(const QString &alternatePhoneNumber)
{
    if (alternatePhoneNumber.trimmed().isEmpty() || m_alternatePhoneNumberModel.contains(alternatePhoneNumber))
        return;

    m_alternatePhoneNumberModel.append(alternatePhoneNumber);
    emit alternatePhoneNumberModelChanged();
}

void QMLDebtTransactionModel::removeAlternatePhoneNumber(int row)
{
    if (row < 0 || row >= m_alternatePhoneNumberModel.count())
        return;

    m_alternatePhoneNumberModel.removeAt(row);
    emit alternatePhoneNumberModelChanged();
}

void QMLDebtTransactionModel::addAddress(const QString &address)
{
    if (address.trimmed().isEmpty() || m_addressModel.contains(address))
        return;

    m_addressModel.append(address);
    emit addressModelChanged();
}

void QMLDebtTransactionModel::removeAddress(int row)
{
    if (row < 0 || row >= m_addressModel.count())
        return;

    m_addressModel.removeAt(row);
    emit addressModelChanged();
}

void QMLDebtTransactionModel::addEmailAddress(const QString &emailAddress)
{
    if (emailAddress.trimmed().isEmpty() || m_emailAddressModel.contains(emailAddress))
        return;

    m_emailAddressModel.append(emailAddress);
    emit emailAddressModelChanged();
}

void QMLDebtTransactionModel::removeEmailAddress(int row)
{
    if (row < 0 || row >= m_emailAddressModel.count())
        return;

    m_emailAddressModel.removeAt(row);
    emit emailAddressModelChanged();
}

void QMLDebtTransactionModel::addDebt(double totalDebt, const QDateTime &dueDateTime, const QString &note)
{
    if (totalDebt <= 0.0 || !dueDateTime.isValid())
        return;

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_transactions.append(DebtTransaction{ totalDebt, dueDateTime, Note{note} });
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
    m_transactions.takeAt(row).clear();
    m_dirty = true;
    endRemoveRows();
}

void QMLDebtTransactionModel::tryQuery()
{
    if (m_debtorId <= 0)
        return;

    setBusy(true);
    emit execute(new DebtorQuery::ViewDebtTransactions(m_debtorId, this));
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
            m_transactions = DebtTransactionList{result.outcome().toMap().value("transactions").toList()};
            setClientId(result.outcome().toMap().value("client_id").toInt());
            setPreferredName(result.outcome().toMap().value("preferred_name").toString());
            setPrimaryPhoneNumber(result.outcome().toMap().value("primary_phone_number").toString());
            setNote(result.outcome().toMap().value("note").toString());

            endResetModel();

            emit success(ViewDebtorTransactionsSuccess);
        } else if (result.request().command() == DebtorQuery::AddDebtor::COMMAND) {
            clearAll();
            emit success(AddDebtorSuccess);
        } else if (result.request().command() == DebtorQuery::AddDebtor::UNDO_COMMAND) {
            clearAll();
            emit success(UndoAddDebtorSuccess);
        } else if (result.request().command() == DebtorQuery::UpdateDebtor::COMMAND) {
            clearAll();
            emit success(UpdateDebtorSuccess);
        } else {
            clearAll();
            emit success(UnknownSuccess);
        }
    } else {
        switch (result.errorCode()) {
        case int(DatabaseError::QueryErrorCode::DuplicateEntryFailure):
            emit error(DuplicateEntryError);
            break;
        case int(DatabaseError::QueryErrorCode::AmountOverpaid):
            emit error(AmountOverpaidError);
            break;
        case int(DatabaseError::QueryErrorCode::InvalidDueDate):
            emit error(InvalidDueDateError);
            break;
        default:
            emit error(UnknownError);
            break;
        }
    }
}

void QMLDebtTransactionModel::clearAll()
{
    setPreferredName(QString());
    setFirstName(QString());
    setLastName(QString());
    setPrimaryPhoneNumber(QString());
    m_alternatePhoneNumberModel = QStringList();
    m_addressModel = QStringList();
    m_emailAddressModel = QStringList();
    m_dirty = false;
}

bool QMLDebtTransactionModel::isExistingDebtor() const
{
    return m_debtorId > 0;
}

bool QMLDebtTransactionModel::paymentsDirty() const
{
    for (const auto &transaction : m_transactions) {
        for (const auto &payment : transaction.debtPayments) {
            Q_UNUSED(payment)
            return true;
        }
    }

    return false;
}

bool QMLDebtTransactionModel::paymentsDueDateValid() const
{
    for (const auto &transaction : m_transactions) {
        for (const auto &payment : transaction.debtPayments) {
            if (payment.dueDateTime <= QDateTime::currentDateTime())
                return false;
        }
    }

    return true;
}

bool QMLDebtTransactionModel::submit()
{
    if (m_primaryPhoneNumber.trimmed().isEmpty()) {
        emit error(NoPrimaryPhoneNumberError);
        return false;
    } else if (m_preferredName.trimmed().isEmpty()) {
        emit error(NoPreferredNameError);
        return false;
    } else if (!m_dirty && !paymentsDirty()) {
        emit error(DataUnchangedError);
        return false;
    } else if (!paymentsDueDateValid()) {
        emit error(InvalidDueDateError);
        return false;
    } else {
        setBusy(true);

        if (isExistingDebtor()) {
            emit execute(new DebtorQuery::UpdateDebtor(DebtorDetails {
                                                           m_debtorId,
                                                           m_clientId,
                                                           m_preferredName,
                                                           m_firstName,
                                                           m_lastName,
                                                           m_primaryPhoneNumber,
                                                           m_imageUrl,
                                                           m_note
                                                       },
                                                       m_transactions,
                                                       this));
        } else {
            emit execute(new DebtorQuery::AddDebtor(DebtorDetails {
                                                        m_preferredName,
                                                        m_firstName,
                                                        m_lastName,
                                                        m_primaryPhoneNumber,
                                                        m_imageUrl,
                                                        m_note
                                                    },
                                                    m_transactions,
                                                    this));
        }
    }

    return true;
}
