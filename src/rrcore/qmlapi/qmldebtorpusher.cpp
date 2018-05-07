#include "qmldebtorpusher.h"

#include "database/databaseexception.h"
#include "database/queryrequest.h"
#include "database/queryresult.h"

QMLDebtorPusher::QMLDebtorPusher(QObject *parent) :
    AbstractPusher(parent)
{

}

QString QMLDebtorPusher::imageSource() const
{
    return m_imageSource;
}

void QMLDebtorPusher::setImageSource(const QString &imageSource)
{
    if (m_imageSource == imageSource)
        return;

    m_imageSource = imageSource;
    emit imageSourceChanged();
}

QString QMLDebtorPusher::firstName() const
{
    return m_firstName;
}

void QMLDebtorPusher::setFirstName(const QString &firstName)
{
    if (m_firstName == firstName)
        return;

    m_firstName = firstName;
    emit firstNameChanged();
}

QString QMLDebtorPusher::lastName() const
{
    return m_lastName;
}

void QMLDebtorPusher::setLastName(const QString &lastName)
{
    if (m_lastName == lastName)
        return;

    m_lastName = lastName;
    emit lastNameChanged();
}

QString QMLDebtorPusher::preferredName() const
{
    return m_preferredName;
}

void QMLDebtorPusher::setPreferredName(const QString &preferredName)
{
    if (m_preferredName == preferredName)
        return;

    m_preferredName = preferredName;
    emit preferredNameChanged();
}

QString QMLDebtorPusher::phoneNumber() const
{
    return m_phoneNumber;
}

void QMLDebtorPusher::setPhoneNumber(const QString phoneNumber)
{
    if (m_phoneNumber == phoneNumber)
        return;

    m_phoneNumber = phoneNumber;
    emit phoneNumberChanged();
}

QString QMLDebtorPusher::address() const
{
    return m_address;
}

void QMLDebtorPusher::setAddress(const QString &address)
{
    if (m_address == address)
        return;

    m_address = address;
    emit addressChanged();
}

QString QMLDebtorPusher::note() const
{
    return m_note;
}

void QMLDebtorPusher::setNote(const QString &note)
{
    if (m_note == note)
        return;

    m_note = note;
    emit noteChanged();
}

void QMLDebtorPusher::processResult(const QueryResult &result)
{
    if (this != result.request().parent())
        return;

    setBusy(false);

    if (result.isSuccessful()) {
        emit success();
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
            emit error();
            break;
        }
    }
}

void QMLDebtorPusher::push()
{
    if (m_phoneNumber.trimmed().isEmpty()) {
        emit error(NoPhoneNumberError);
    } else if (m_preferredName.trimmed().isEmpty()) {
        emit error(NoPreferredNameError);
    } /*else if (m_debtTransactions.isEmpty()) {
        emit error(NoDebtError);
    } */else {
        setBusy(true);

        QVariantMap params;
        params.insert("image_source", m_imageSource);
        params.insert("first_name", m_firstName);
        params.insert("last_name", m_lastName);
        params.insert("preferred_name", m_preferredName);
        params.insert("phone_number", m_phoneNumber);
        params.insert("address", m_address);
        params.insert("note", m_note);
        params.insert("debt_transactions", convertToVariant(m_debtTransactions));

        QueryRequest request(this);
        request.setCommand("add_debtor", params, QueryRequest::Debtor);
        emit executeRequest(request);
    }
}

void QMLDebtorPusher::addDebt(double totalDebt, const QDateTime &dueDateTime, const QString &note)
{
    m_debtTransactions.append(new DebtTransaction{totalDebt, dueDateTime, note, QList<DebtPayment *>()});
}

void QMLDebtorPusher::updateDebt(int debtIndex, double totalDebt, const QDateTime &dueDateTime, const QString &note)
{
    if (debtIndex < 0 || debtIndex >= m_debtTransactions.count()) {
        qWarning() << "Out of range value in:" << Q_FUNC_INFO;
        return;
    }

    DebtTransaction *debtTransaction = m_debtTransactions.takeAt(debtIndex);
    qDeleteAll(debtTransaction->debtPayments);
    delete debtTransaction;
    m_debtTransactions.insert(debtIndex, new DebtTransaction{totalDebt, dueDateTime, note, QList<DebtPayment *>()});
}

void QMLDebtorPusher::removeDebt(int debtIndex)
{
    if (debtIndex < 0 || debtIndex >= m_debtTransactions.count()) {
        qWarning() << "Out of range value in:" << Q_FUNC_INFO;
        return;
    }

    DebtTransaction *debtTransaction = m_debtTransactions.takeAt(debtIndex);
    qDeleteAll(debtTransaction->debtPayments);
    delete debtTransaction;
}

void QMLDebtorPusher::addPayment(int debtIndex, double amount, const QString &note)
{
    if (debtIndex < 0 || debtIndex >= m_debtTransactions.count()) {
        qWarning() << "Out of range value in:" << Q_FUNC_INFO;
        return;
    }

    m_debtTransactions.at(debtIndex)->debtPayments.append(new DebtPayment{amount, note});
}

void QMLDebtorPusher::updatePayment(int debtIndex, int paymentIndex, double amount, const QString &note)
{
    if ((debtIndex < 0 || debtIndex >= m_debtTransactions.count())
            && (paymentIndex < 0 || paymentIndex >= m_debtTransactions.at(debtIndex)->debtPayments.count())) {
        qWarning() << "Out of range value in:" << Q_FUNC_INFO;
        return;
    }

    delete m_debtTransactions.at(debtIndex)->debtPayments.takeAt(paymentIndex);
    m_debtTransactions.at(debtIndex)->debtPayments.insert(paymentIndex, new DebtPayment{amount, note});
}

void QMLDebtorPusher::removePayment(int debtIndex, int paymentIndex)
{
    if ((debtIndex < 0 || debtIndex >= m_debtTransactions.count())
            && (paymentIndex < 0 || paymentIndex >= m_debtTransactions.at(debtIndex)->debtPayments.count())) {
        qWarning() << "Out of range value in:" << Q_FUNC_INFO;
        return;
    }

    delete m_debtTransactions.at(debtIndex)->debtPayments.takeAt(paymentIndex);
}

QVariant QMLDebtorPusher::convertToVariant(const QList<DebtTransaction *> &debtTransactions)
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
