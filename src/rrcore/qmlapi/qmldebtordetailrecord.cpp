#include "qmldebtordetailrecord.h"
#include "database/databasethread.h"
#include "queryexecutors/debtor.h"
#include "utility/debtorutils.h"
#include <QDateTime>

QMLDebtorDetailRecord::QMLDebtorDetailRecord(QObject *parent) :
    AbstractDetailRecord(DatabaseThread::instance(), parent)
{ }

QMLDebtorDetailRecord::QMLDebtorDetailRecord(DatabaseThread &thread, QObject *parent) :
    AbstractDetailRecord(thread, parent)
{
    connect(this, &QMLDebtorDetailRecord::debtorIdChanged,
            this, &QMLDebtorDetailRecord::tryQuery);
}

int QMLDebtorDetailRecord::debtorId() const
{
    return m_debtor.id;
}

void QMLDebtorDetailRecord::setDebtorId(int debtorId)
{
    if (m_debtor.id == debtorId)
        return;

    m_debtor.id = debtorId;
    emit debtorIdChanged();
}

QString QMLDebtorDetailRecord::preferredName() const
{
    return m_debtor.client.preferredName;
}

void QMLDebtorDetailRecord::setPreferredName(const QString &preferredName)
{
    if (m_debtor.client.preferredName == preferredName)
        return;

    m_debtor.client.preferredName = preferredName;
    emit preferredNameChanged();
}

QString QMLDebtorDetailRecord::firstName() const
{
    return m_debtor.client.firstName;
}

void QMLDebtorDetailRecord::setFirstName(const QString &firstName)
{
    if (m_debtor.client.firstName == firstName)
        return;

    m_debtor.client.firstName = firstName;
    emit firstNameChanged();
}

QString QMLDebtorDetailRecord::lastName() const
{
    return m_debtor.client.lastName;
}

void QMLDebtorDetailRecord::setLastName(const QString &lastName)
{
    if (m_debtor.client.lastName == lastName)
        return;

    m_debtor.client.lastName = lastName;
    emit lastNameChanged();
}

QString QMLDebtorDetailRecord::phoneNumber() const
{
    return m_debtor.client.phoneNumber;
}

void QMLDebtorDetailRecord::setPhoneNumber(const QString &phoneNumber)
{
    if (m_debtor.client.phoneNumber == phoneNumber)
        return;

    m_debtor.client.phoneNumber = phoneNumber;
    emit phoneNumberChanged();
}

QStringList QMLDebtorDetailRecord::addressModel() const
{
    return QStringList();
}

void QMLDebtorDetailRecord::setAddressModel(const QStringList &addressModel)
{
    Q_UNUSED(addressModel);
}

QStringList QMLDebtorDetailRecord::emailModel() const
{
    return QStringList();
}

void QMLDebtorDetailRecord::setEmailModel(const QStringList &emailModel)
{
    Q_UNUSED(emailModel);
}

QUrl QMLDebtorDetailRecord::imageUrl() const
{
    return m_debtor.client.imageUrl;
}

void QMLDebtorDetailRecord::setImageUrl(const QUrl &imageUrl)
{
    if (m_debtor.client.imageUrl == imageUrl)
        return;

    m_debtor.client.imageUrl = imageUrl;
    emit imageUrlChanged();
}

QDateTime QMLDebtorDetailRecord::created() const
{
    return m_debtor.timestamp.created;
}

void QMLDebtorDetailRecord::setCreated(const QDateTime &created)
{
    if (m_debtor.timestamp.created == created)
        return;

    m_debtor.timestamp.created = created;
    emit createdChanged();
}

QDateTime QMLDebtorDetailRecord::lastEdited() const
{
    return m_debtor.timestamp.lastEdited;
}

void QMLDebtorDetailRecord::setLastEdited(const QDateTime &lastEdited)
{
    if (m_debtor.timestamp.lastEdited == lastEdited)
        return;

    m_debtor.timestamp.lastEdited = lastEdited;
    emit lastEditedChanged();
}

int QMLDebtorDetailRecord::userId() const
{
    return m_debtor.user.id;
}

void QMLDebtorDetailRecord::setUserId(int userId)
{
    if (m_debtor.user.id == userId)
        return;

    m_debtor.user.id = userId;
    emit userIdChanged();
}

QString QMLDebtorDetailRecord::user() const
{
    return m_debtor.user.user;
}

void QMLDebtorDetailRecord::setUser(const QString &user)
{
    if (m_debtor.user.user == user)
        return;

    m_debtor.user.user = user;
    emit userChanged();
}

void QMLDebtorDetailRecord::tryQuery()
{
    if (m_debtor.id <= 0)
        return;

    setBusy(true);
    emit execute(new DebtorQuery::FetchDebtor(m_debtor.id,
                                              this));
}

void QMLDebtorDetailRecord::processResult(const QueryResult result)
{
    if (result.request().receiver() != this)
        return;

    setBusy(false);

    if (result.isSuccessful()) {
        if (result.request().command() == DebtorQuery::FetchDebtor::COMMAND) {
            const Utility::Debtor debtor{ result.outcome().toMap() };
            setPreferredName(debtor.client.preferredName);
            setFirstName(debtor.client.firstName);
            setLastName(debtor.client.lastName);
            setImageUrl(debtor.client.imageUrl);
            setPhoneNumber(debtor.client.phoneNumber);
            setCreated(debtor.timestamp.created);
            setLastEdited(debtor.timestamp.lastEdited);
            setUserId(debtor.user.id);
            setUser(debtor.user.user);

            emit success();
        }
    } else {
        emit error();
    }
}
