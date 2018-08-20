﻿#include "qmldebttransactionmodel.h"
#include "models/debtpaymentmodel.h"
#include "database/databaseexception.h"

#include <QDateTime>

QMLDebtTransactionModel::QMLDebtTransactionModel(QObject *parent) :
    AbstractVisualListModel(parent),
    m_debtorId(-1),
    m_clientId(-1),
    m_dirty(false)
{
    connect(this, &QMLDebtTransactionModel::debtorIdChanged, this, &QMLDebtTransactionModel::tryQuery);
}

QMLDebtTransactionModel::~QMLDebtTransactionModel()
{
    clearAll();
}

int QMLDebtTransactionModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_existingDebtTransactions.count() + m_newDebtTransactions.count();
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
            return m_newDebtTransactions.at(index.row() - m_records.count())->note;
        break;
    case DueDateRole:
        if (isExistingRecord(index.row()))
            return m_existingDebtTransactions.at(index.row())->dueDateTime;
        else
            return m_newDebtTransactions.at(index.row() - m_records.count())->dueDateTime;
        break;
    case CreatedRole:
        if (isExistingRecord(index.row()))
            return m_records.at(index.row()).toMap().value("created").toDateTime();
        else
            return QDateTime();
        break;
    case PaymentModelRole:
        return QVariant::fromValue<QObject *>(m_debtPaymentModels.at(index.row()));
        break;
    case CurrentBalanceRole: {
        if (isExistingRecord(index.row()))
            return m_existingDebtTransactions.at(index.row())->totalDebt;
        else
            return m_newDebtTransactions.at(index.row() - m_records.count())->totalDebt;
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
    DebtPayment *debtPayment = new DebtPayment{ -1, 0.0, QString(), DebtPayment::State::New }; // Add first dummy payment
    m_newDebtTransactions.append(new DebtTransaction{ -1, totalDebt, dueDateTime,
                                                      note, { debtPayment }, DebtTransaction::State::New });
    endInsertRows();

    DebtPaymentModel *debtPaymentModel = new DebtPaymentModel(this);
    debtPaymentModel->addPayment(debtPayment);
    m_debtPaymentModels.append(debtPaymentModel);

    return rowCount() - 1;
}

void QMLDebtTransactionModel::updateDebt(int debtIndex, const QDateTime &dueDateTime, const QString &note)
{
    if (!index(debtIndex).isValid()) {
        qWarning() << "Debt index out of range value in:" << Q_FUNC_INFO;
        return;
    }

    DebtTransaction *debtTransaction = nullptr;
    if (isExistingRecord(debtIndex))
        debtTransaction = m_existingDebtTransactions.at(debtIndex);
    else
        debtTransaction = m_newDebtTransactions.at(debtIndex - m_records.count());

    debtTransaction->dueDateTime = dueDateTime;
    debtTransaction->note = note;
    debtTransaction->state = DebtTransaction::State::Dirty;
    setDirty(true);

    emit dataChanged(index(debtIndex), index(debtIndex));
}

void QMLDebtTransactionModel::removeDebt(int debtIndex)
{
    if (!index(debtIndex).isValid()) {
        qWarning() << "Debt index out of range value in:" << Q_FUNC_INFO;
        return;
    }

    DebtTransaction *debtTransaction = nullptr;
    DebtPaymentModel *debtPaymentModel = m_debtPaymentModels.takeAt(debtIndex);

    if (isExistingRecord(debtIndex))
        debtTransaction = m_existingDebtTransactions.takeAt(debtIndex);
    else
        debtTransaction = m_newDebtTransactions.takeAt(debtIndex);


    beginRemoveRows(QModelIndex(), debtIndex, debtIndex);
    qDeleteAll(debtTransaction->debtPayments);
    debtPaymentModel->deleteLater();

    if (debtTransaction->state != DebtTransaction::State::New)
        m_archivedDebtTransactionIds.append(debtTransaction->id);

    setDirty(true);

    delete debtTransaction;
    endRemoveRows();
}

void QMLDebtTransactionModel::addPayment(int debtIndex, double amount, const QString &note)
{
    if (!index(debtIndex).isValid()) {
        qWarning() << "Debt index out of range value in:" << Q_FUNC_INFO;
        return;
    }

    if (isExistingRecord(debtIndex)) {
        DebtTransaction *debtTransaction = m_existingDebtTransactions.at(debtIndex);
        DebtPayment *debtPayment = new DebtPayment{ -1, amount, note, DebtPayment::State::New };
        debtTransaction->debtPayments.append(debtPayment);
        debtTransaction->totalDebt -= debtPayment->amount;
        debtTransaction->state = DebtTransaction::State::Dirty;
        m_debtPaymentModels.at(debtIndex)->addPayment(debtPayment);
    } else {
        debtIndex += m_existingDebtTransactions.count();
        DebtTransaction *debtTransaction = m_newDebtTransactions.at(debtIndex);
        DebtPayment *debtPayment = new DebtPayment{ -1, amount, note, DebtPayment::State::New };
        debtTransaction->debtPayments.append(debtPayment);
        debtTransaction->totalDebt -= debtPayment->amount;
        debtTransaction->state = DebtTransaction::State::Dirty;
        m_debtPaymentModels.at(debtIndex)->addPayment(debtPayment);
    }

    setDirty(true);
    emit dataChanged(index(debtIndex), index(debtIndex));
}

void QMLDebtTransactionModel::updatePayment(int debtIndex, int paymentIndex, double amount, const QString &note)
{
    if (!index(debtIndex).isValid()) {
        qWarning() << "Debt index out of range value in:" << Q_FUNC_INFO;
        return;
    }
    if (!m_debtPaymentModels.at(debtIndex)->index(paymentIndex).isValid()) {
        qWarning() << "Payment index out of range value in:" << Q_FUNC_INFO;
        return;
    }
    if (amount < 0.0)
        return;

    DebtTransaction *debtTransaction = m_newDebtTransactions.at(debtIndex);
    debtTransaction->state = DebtTransaction::State::Dirty;

    DebtPayment *debtPayment = m_newDebtTransactions.at(debtIndex)->debtPayments.at(paymentIndex);
    const double oldAmount = debtPayment->amount;
    const QString &oldNote = debtPayment->note;

    if (oldAmount != amount || oldNote != note) {
        debtPayment->amount = amount;
        debtPayment->note = note;
        debtPayment->state = DebtPayment::State::Dirty;

        m_debtPaymentModels.at(debtIndex)->updatePayment(debtPayment);
        debtTransaction->totalDebt += (oldAmount - debtPayment->amount);
        setDirty(true);
        emit dataChanged(index(debtIndex), index(debtIndex));
    }
}

void QMLDebtTransactionModel::removePayment(int debtIndex, int paymentIndex)
{
    if (!index(debtIndex).isValid()) {
        qWarning() << "Debt index out of range value in:" << Q_FUNC_INFO;
        return;
    }
    if (!m_debtPaymentModels.at(debtIndex)->index(paymentIndex).isValid()) {
        qWarning() << "Payment index out of range value in:" << Q_FUNC_INFO;
        return;
    }

    DebtTransaction *debtTransaction = nullptr;
    DebtPayment *debtPayment = nullptr;
    if (isExistingRecord(debtIndex))
        debtTransaction = m_existingDebtTransactions.at(debtIndex);
    else
        debtTransaction = m_newDebtTransactions.at(debtIndex);
    debtPayment = debtTransaction->debtPayments.takeAt(paymentIndex);

    m_debtPaymentModels.at(debtIndex)->removePayment(debtPayment);
    debtTransaction->totalDebt += debtPayment->amount;
    debtTransaction->state = DebtTransaction::State::Dirty;

    if (debtPayment->state != DebtPayment::State::New)
        m_archivedDebtPaymentIds.append(debtPayment->id);

    setDirty(true);

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

    disconnect(this, &QMLDebtTransactionModel::debtorIdChanged, this, &QMLDebtTransactionModel::tryQuery);

    setBusy(false);

    if (result.isSuccessful()) {
        if (result.request().command() == "view_debt_transactions") {
            beginResetModel();

            clearAll();

            m_records = result.outcome().toMap().value("transactions").toList();
            const QVariantList paymentGroups = result.outcome().toMap().value("payment_groups").toList();

            setDebtorId(result.outcome().toMap().value("debtor_id").toInt());
            setClientId(result.outcome().toMap().value("client_id").toInt());
            setPreferredName(result.outcome().toMap().value("preferred_name").toString());
            setPrimaryPhoneNumber(result.outcome().toMap().value("primary_phone_number").toString());
            setNote(result.outcome().toMap().value("note").toString());

            for (int i = 0; i < m_records.count(); ++i) {
                const QVariantMap &transaction = m_records.at(i).toMap();
                const QVariantList &paymentRecords = paymentGroups.at(i).toList();

                DebtPaymentModel *model = new DebtPaymentModel(this);

                model->setTransactionId(paymentRecords.first().toMap().value("transaction_id").toInt());
                model->setTotalAmount(0.0);
                model->setPaymentRecords(paymentRecords);
                m_debtPaymentModels.append(model);

                QList<DebtPayment *> debtPayments;
                for (int j = 0; j < paymentRecords.count(); ++j) {
                    const QVariantMap &paymentRecord = paymentRecords.at(i).toMap();
                    DebtPayment *debtPayment = new DebtPayment{
                            paymentRecord.value("debt_payment_id").toInt(),
                            paymentRecord.value("amount").toDouble(),
                            paymentRecord.value("note").toString(),
                            DebtPayment::State::Clean };
                    debtPayments.append(debtPayment);
                }

                DebtTransaction *debtTransaction = new DebtTransaction {
                        transaction.value("debt_transaction_id").toInt(),
                        transaction.value("total_debt").toDouble(),
                        transaction.value("due_date").toDateTime(),
                        transaction.value("note").toString(),
                        debtPayments,
                        DebtTransaction::State::Clean };

                m_existingDebtTransactions.append(debtTransaction);
            }

            endResetModel();

            emit success(ViewDebtorTransactionsSuccess);
        } else if (result.request().command() == "add_new_debtor") {
            clearAll();
            emit success(AddDebtorSuccess);
        } else if (result.request().command() == "undo_add_new_debtor") {
            clearAll();
            emit success(UndoAddDebtorSuccess);
        } else if (result.request().command() == "update_debtor") {
            clearAll();
            emit success(UpdateDebtorSuccess);
        } else {
            clearAll();
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

    connect(this, &QMLDebtTransactionModel::debtorIdChanged, this, &QMLDebtTransactionModel::tryQuery);
}

QVariant QMLDebtTransactionModel::convertToVariant(const QList<DebtTransaction *> &debtTransactions)
{
    QVariantList debtTransactionList;
    for (const DebtTransaction *transaction : debtTransactions) {
        QVariantMap debtTransactionMap;

        if (transaction->state != DebtTransaction::State::Clean) {
            debtTransactionMap.insert("debt_transaction_id", transaction->id);
            debtTransactionMap.insert("total_debt", transaction->totalDebt);
            debtTransactionMap.insert("due_date", transaction->dueDateTime);
            debtTransactionMap.insert("note", transaction->note);
        }

        QVariantList updatedDebtPaymentList, newDebtPaymentList;
        for (const DebtPayment *debtPayment : transaction->debtPayments) {
            if (debtPayment->state == DebtPayment::State::Dirty)
                updatedDebtPaymentList.append(QVariantMap{ { "debt_payment_id", debtPayment->id },
                                                           { "amount_paid", debtPayment->amount },
                                                           { "note", debtPayment->note } });
            if (debtPayment->state == DebtPayment::State::New)
                newDebtPaymentList.append(QVariantMap{ { "amount_paid", debtPayment->amount }, { "note", debtPayment->note } });
        }

        if (!updatedDebtPaymentList.isEmpty())
            debtTransactionMap.insert("updated_debt_payments", updatedDebtPaymentList);
        if (!newDebtPaymentList.isEmpty())
            debtTransactionMap.insert("new_debt_payments", newDebtPaymentList);

        debtTransactionList.append(debtTransactionMap);
    }

    return debtTransactionList;
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

    clearDebtTransactions();
    clearPayments();
    setDirty(false);
}

bool QMLDebtTransactionModel::submit()
{
    if (m_primaryPhoneNumber.trimmed().isEmpty()) {
        emit error(NoPrimaryPhoneNumberError);
        return false;
    } else if (m_preferredName.trimmed().isEmpty()) {
        emit error(NoPreferredNameError);
        return false;
    } else if (m_existingDebtTransactions.isEmpty() && m_newDebtTransactions.isEmpty()) {
        emit error(NoDebtError);
        return false;
    } else if ((!m_existingDebtTransactions.isEmpty() && m_existingDebtTransactions.first()->debtPayments.isEmpty())
               && (!m_newDebtTransactions.first()->debtPayments.isEmpty() && m_newDebtTransactions.isEmpty())) {
        emit error(NoPaymentError);
        return false;
    } else {
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

        if (!m_newDebtTransactions.isEmpty() && !isDirty()) {
            params.insert("new_debt_transactions", convertToVariant(m_newDebtTransactions));

            QueryRequest request(this);
            request.setCommand("add_new_debtor", params, QueryRequest::Debtor);
            emit executeRequest(request);
        } else {
            params.insert("debtor_id", m_debtorId);
            params.insert("client_id", m_clientId);
            params.insert("updated_debt_transactions", convertToVariant(m_existingDebtTransactions));
            params.insert("new_debt_transactions", convertToVariant(m_newDebtTransactions));

            QueryRequest request(this);
            request.setCommand("update_debtor", params, QueryRequest::Debtor);
            emit executeRequest(request);
        }
    }

    return true;
}

void QMLDebtTransactionModel::clearDebtTransactions()
{
    while (m_existingDebtTransactions.count()) {
        DebtTransaction *debtTransaction = m_existingDebtTransactions.takeFirst();
        qDeleteAll(debtTransaction->debtPayments);
        delete debtTransaction;
    }

    while (m_newDebtTransactions.count()) {
        DebtTransaction *debtTransaction = m_newDebtTransactions.takeFirst();
        qDeleteAll(debtTransaction->debtPayments);
        delete debtTransaction;
    }
}

bool QMLDebtTransactionModel::isExistingRecord(int row) const
{
    return row >= 0 && row < m_existingDebtTransactions.count();
}

bool QMLDebtTransactionModel::isDirty() const
{
    return m_dirty;
}

void QMLDebtTransactionModel::setDirty(bool dirty)
{
    if (m_dirty == dirty)
        return;

    m_dirty = dirty;
    emit dirtyChanged();
}
