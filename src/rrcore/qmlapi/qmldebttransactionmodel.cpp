#include "qmldebttransactionmodel.h"
#include "models/debtpaymentmodel.h"
#include "database/databaseexception.h"

#include <QDateTime>

QMLDebtTransactionModel::QMLDebtTransactionModel(QObject *parent) :
    AbstractVisualListModel(parent),
    m_debtorId(-1)
{
    connect(this, &QMLDebtTransactionModel::debtorIdChanged, this, &QMLDebtTransactionModel::tryQuery);
}

QMLDebtTransactionModel::~QMLDebtTransactionModel()
{
    for (DebtTransaction *debtTransaction : m_debtTransactions) {
        qDeleteAll(debtTransaction->debtPayments);
        debtTransaction->debtPayments.clear();
    }

    qDeleteAll(m_debtTransactions);
    m_debtTransactions.clear();
}

int QMLDebtTransactionModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_records.count() + m_debtTransactions.count();
}

QVariant QMLDebtTransactionModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case TransactionIdRole:
        if (isExistingRecord(index.row()))
            return m_records.at(index.row()).toMap().value("transaction_id").toInt();
        else
            return -1;
        break;
    case RelatedTransactionRole: {
        if (isExistingRecord(index.row())) {
            const QString &relatedTransaction = m_records.at(index.row()).toMap().value("related_transaction_table").toString();
            if (relatedTransaction == "sale_transaction")
                return tr("Sales");
            else
                return relatedTransaction;
        }
    }
        break;
    case RelatedTransactionIdRole:
        if (isExistingRecord(index.row()))
            return m_records.at(index.row()).toMap().value("related_transaction_id").toInt();
        else
            return -1;
        break;
    case NoteRole:
        if (isExistingRecord(index.row()))
            return m_records.at(index.row()).toMap().value("note_id").toString();
        else
            return m_debtTransactions.at(index.row() - m_records.count())->note;
        break;
    case DueDateRole:
        if (isExistingRecord(index.row()))
            return m_records.at(index.row()).toMap().value("due_date").toString();
        else
            return m_debtTransactions.at(index.row() - m_records.count())->dueDateTime;
        break;
    case CreatedRole:
        if (isExistingRecord(index.row()))
            return m_records.at(index.row()).toMap().value("created").toDateTime();
        else
            return QDateTime::currentDateTime();
        break;
    case PaymentModelRole:
        return QVariant::fromValue<QObject *>(m_debtPaymentModels.at(index.row()));
        break;
    case CurrentBalanceRole: {
        if (isExistingRecord(index.row())) {
            DebtPaymentModel *model = m_debtPaymentModels.at(index.row());
            return model->data(model->index(model->rowCount() - 1), DebtPaymentModel::BalanceRole).toDouble();
        } else {
            return m_debtTransactions.at(index.row() - m_records.count())->totalDebt;
        }
    }
        break;
    }

    return QVariant();
}

QHash<int, QByteArray> QMLDebtTransactionModel::roleNames() const
{
    QHash<int, QByteArray> roles(AbstractVisualListModel::roleNames());
    roles.insert(TransactionIdRole, "transaction_id");
    roles.insert(RelatedTransactionRole, "related_transaction");
    roles.insert(RelatedTransactionIdRole, "related_transaction_id");
    roles.insert(NoteRole, "note");
    roles.insert(DueDateRole, "due_date");
    roles.insert(CreatedRole, "created");
    roles.insert(PaymentModelRole, "payment_model");
    roles.insert(CurrentBalanceRole, "current_balance");

    return roles;
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

QString QMLDebtTransactionModel::imageSource() const
{
    return m_imageSource;
}

void QMLDebtTransactionModel::setImageSource(const QString &imageSource)
{
    if (m_imageSource == imageSource)
        return;

    m_imageSource = imageSource;
    emit imageSourceChanged();
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
    return m_note;
}

void QMLDebtTransactionModel::setNote(const QString &note)
{
    if (m_note == note)
        return;

    m_note = note;
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

int QMLDebtTransactionModel::addDebt(double totalDebt, const QDateTime &dueDateTime, const QString &note)
{
    if (totalDebt <= 0.0 || !dueDateTime.isValid())
        return -1;

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    DebtPayment *debtPayment = new DebtPayment{ 0.0, QString() }; // Add first dummy payment
    m_debtTransactions.append(new DebtTransaction{ totalDebt, dueDateTime, note, { debtPayment } });
    endInsertRows();

    DebtPaymentModel *debtPaymentModel = new DebtPaymentModel(this);
    debtPaymentModel->addPayment(debtPayment);
    m_debtPaymentModels.append(debtPaymentModel);

    return rowCount() - 1;
}

void QMLDebtTransactionModel::updateDebt(int debtIndex, const QDateTime &dueDateTime, const QString &note)
{
    if (debtIndex < 0 || debtIndex >= m_debtTransactions.count()) {
        qWarning() << "Out of range value in:" << Q_FUNC_INFO;
        return;
    }

    DebtTransaction *debtTransaction = m_debtTransactions.at(debtIndex);
    debtTransaction->dueDateTime = dueDateTime;
    debtTransaction->note = note;

    emit dataChanged(index(debtIndex), index(debtIndex));
}

void QMLDebtTransactionModel::removeDebt(int debtIndex)
{
    if (debtIndex < 0 || debtIndex >= m_debtTransactions.count()) {
        qWarning() << "Out of range value in:" << Q_FUNC_INFO;
        return;
    }

    DebtTransaction *debtTransaction = m_debtTransactions.takeAt(debtIndex);
    DebtPaymentModel *debtPaymentModel = m_debtPaymentModels.takeAt(debtIndex);

    beginRemoveRows(QModelIndex(), debtIndex, debtIndex);
    qDeleteAll(debtTransaction->debtPayments);
    debtPaymentModel->deleteLater();
    delete debtTransaction;
    endRemoveRows();
}

void QMLDebtTransactionModel::addPayment(int debtIndex, double amount, const QString &note)
{
    if (debtIndex < 0 || debtIndex >= m_debtTransactions.count()) {
        qWarning() << "Out of range value in:" << Q_FUNC_INFO;
        return;
    }

    DebtTransaction *debtTransaction = m_debtTransactions.at(debtIndex);
    DebtPayment *debtPayment = new DebtPayment{ amount, note };
    debtTransaction->debtPayments.append(debtPayment);
    debtTransaction->totalDebt -= debtPayment->amount;
    m_debtPaymentModels.at(debtIndex)->addPayment(debtPayment);
    emit dataChanged(index(debtIndex), index(debtIndex));
}

void QMLDebtTransactionModel::updatePayment(int debtIndex, int paymentIndex, double amount, const QString &note)
{
    if ((debtIndex < 0 || debtIndex >= m_debtTransactions.count())
            && (paymentIndex < 0 || paymentIndex >= m_debtTransactions.at(debtIndex)->debtPayments.count())) {
        qWarning() << "Out of range value in:" << Q_FUNC_INFO;
        return;
    }
    if (amount < 0.0)
        return;

    DebtTransaction *debtTransaction = m_debtTransactions.at(debtIndex);
    DebtPayment *debtPayment = m_debtTransactions.at(debtIndex)->debtPayments.at(paymentIndex);
    const double oldAmount = debtPayment->amount;
    const QString &oldNote = debtPayment->note;

    if (oldAmount != amount || oldNote != note) {
        debtPayment->amount = amount;
        debtPayment->note = note;

        m_debtPaymentModels.at(debtIndex)->updatePayment(debtPayment);
        debtTransaction->totalDebt += (oldAmount - debtPayment->amount);
        emit dataChanged(index(debtIndex), index(debtIndex));
    }
}

void QMLDebtTransactionModel::removePayment(int debtIndex, int paymentIndex)
{
    if ((debtIndex < 0 || debtIndex >= m_debtTransactions.count())
            && (paymentIndex < 0 || paymentIndex >= m_debtTransactions.at(debtIndex)->debtPayments.count())) {
        qWarning() << "Out of range value in:" << Q_FUNC_INFO;
        return;
    }

    DebtPayment *debtPayment = m_debtTransactions.at(debtIndex)->debtPayments.takeAt(paymentIndex);
    m_debtPaymentModels.at(debtIndex)->removePayment(debtPayment);
    m_debtTransactions.at(debtIndex)->totalDebt += debtPayment->amount;
    delete debtPayment;
    emit dataChanged(index(debtIndex), index(debtIndex));
}

void QMLDebtTransactionModel::clearPayments()
{
    if (!m_debtPaymentModels.isEmpty()) {
        qDeleteAll(m_debtPaymentModels);
        m_debtPaymentModels.clear();
    }
}

void QMLDebtTransactionModel::tryQuery()
{
    if (m_debtorId <= 0)
        return;

    setBusy(true);
    QueryRequest request(this);
    request.setCommand("view_debt_transactions", { { "debtor_id", m_debtorId } }, QueryRequest::Debtor);
    emit executeRequest(request);
}

void QMLDebtTransactionModel::processResult(const QueryResult result)
{
    if (result.request().receiver() != this)
        return;

    setBusy(false);

    if (result.isSuccessful()) {
        if (result.request().command() == "view_debt_transactions") {
            beginResetModel();

            clearPayments();

            m_records = result.outcome().toMap().value("transactions").toList();
            const QVariantList paymentGroups = result.outcome().toMap().value("payment_groups").toList();

            for (const QVariant &paymentGroup : paymentGroups) {
                const QVariantList &paymentRecords = paymentGroup.toList();

                DebtPaymentModel *model = new DebtPaymentModel(this);

                model->setTransactionId(paymentRecords.first().toMap().value("transaction_id").toInt());
                model->setTotalAmount(0.0);
                model->setPaymentRecords(paymentRecords);
                m_debtPaymentModels.append(model);
            }

            endResetModel();

            emit success(ViewDebtorTransactionsSuccess);
        } else if (result.request().command() == "add_new_debtor") {
            resetAll();
            clearDebtTransactions();
            emit success(AddDebtorSuccess);
        } else if (result.request().command() == "undo_add_new_debtor") {
            resetAll();
            clearDebtTransactions();
            emit success(UndoAddDebtorSuccess);
        } else {
            resetAll();
            clearDebtTransactions();
            emit success(UnknownSuccess);
        }
    } else {
        switch (result.errorCode()) {
        case int(DatabaseException::RRErrorCode::DuplicateEntryFailure):
            emit error(DuplicateEntryError);
            break;
        case int(DatabaseException::RRErrorCode::AmountOverpaid):
            emit error(AmountOverpaidError);
            break;
        case int(DatabaseException::RRErrorCode::InvalidDueDate):
            emit error(InvalidDueDateError);
            break;
        default:
            emit error(UnknownError);
            break;
        }
    }
}

QVariant QMLDebtTransactionModel::convertToVariant(const QList<DebtTransaction *> &debtTransactions)
{
    QVariantList debtTransactionList;
    for (const DebtTransaction *transaction : debtTransactions) {
        QVariantMap debtTransactionMap;
        debtTransactionMap.insert("total_debt", transaction->totalDebt);
        debtTransactionMap.insert("due_date_time", transaction->dueDateTime);
        debtTransactionMap.insert("note", transaction->note);

        QVariantList paymentAmountList;
        for (const DebtPayment *debtPayment : transaction->debtPayments)
            paymentAmountList.append(QVariantMap{ { "amount", debtPayment->amount }, { "note", debtPayment->note } });

        debtTransactionMap.insert("debt_payments", paymentAmountList);
        debtTransactionList.append(debtTransactionMap);
    }

    return debtTransactionList;
}

void QMLDebtTransactionModel::resetAll()
{
    setDebtorId(-1);
    setPreferredName(QString());
    setFirstName(QString());
    setLastName(QString());
    setPrimaryPhoneNumber(QString());
    m_alternatePhoneNumberModel = QStringList();
    m_addressModel = QStringList();
    m_emailAddressModel = QStringList();
}

bool QMLDebtTransactionModel::submit()
{
    if (m_primaryPhoneNumber.trimmed().isEmpty()) {
        emit error(NoPrimaryPhoneNumberError);
        return false;
    } else if (m_preferredName.trimmed().isEmpty()) {
        emit error(NoPreferredNameError);
        return false;
    } else if (m_debtTransactions.isEmpty()) {
        emit error(NoDebtError);
        return false;
    } else if (m_debtTransactions.first()->debtPayments.isEmpty()) {
        emit error(NoPaymentError);
        return false;
    }else {
        setBusy(true);

        QVariantMap params;
        params.insert("can_undo", true);
        params.insert("image_source", m_imageSource);
        params.insert("first_name", m_firstName);
        params.insert("last_name", m_lastName);
        params.insert("preferred_name", m_preferredName);
        params.insert("primary_phone_number", m_primaryPhoneNumber);
        params.insert("alternate_phone_numbers", QVariant(m_alternatePhoneNumberModel));
        params.insert("addresses", QVariant(m_addressModel));
        params.insert("email_addresses", QVariant(m_emailAddressModel));
        params.insert("note", m_note);
        params.insert("debt_transactions", convertToVariant(m_debtTransactions));

        QueryRequest request(this);
        request.setCommand("add_new_debtor", params, QueryRequest::Debtor);
        emit executeRequest(request);
    }

    return true;
}

void QMLDebtTransactionModel::clearDebtTransactions()
{
    while (m_debtTransactions.count()) {
        DebtTransaction *debtTransaction = m_debtTransactions.takeFirst();
        qDeleteAll(debtTransaction->debtPayments);
        delete debtTransaction;
    }
}

bool QMLDebtTransactionModel::isExistingRecord(int row) const
{
    return row >= 0 && row < m_records.count();
}

