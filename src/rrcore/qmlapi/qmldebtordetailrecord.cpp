#include "qmldebtordetailrecord.h"
#include "database/databasethread.h"
#include "queryexecutors/debtor.h"
#include <QDateTime>

QMLDebtorDetailRecord::QMLDebtorDetailRecord(QObject *parent) :
    AbstractDetailRecord(DatabaseThread::instance(), parent)
{}

QMLDebtorDetailRecord::QMLDebtorDetailRecord(DatabaseThread &thread, QObject *parent) :
    AbstractDetailRecord(thread, parent),
    m_debtorId(-1),
    m_userId(-1)
{
    connect(this, &QMLDebtorDetailRecord::debtorIdChanged, this, &QMLDebtorDetailRecord::tryQuery);
}

int QMLDebtorDetailRecord::debtorId() const
{
    return m_debtorId;
}

void QMLDebtorDetailRecord::setDebtorId(int debtorId)
{
    if (m_debtorId == debtorId)
        return;

    m_debtorId = debtorId;
    emit debtorIdChanged();
}

QString QMLDebtorDetailRecord::preferredName() const
{
    return m_preferredName;
}

void QMLDebtorDetailRecord::setPreferredName(const QString &preferredName)
{
    if (m_preferredName == preferredName)
        return;

    m_preferredName = preferredName;
    emit preferredNameChanged();
}

QString QMLDebtorDetailRecord::firstName() const
{
    return m_firstName;
}

void QMLDebtorDetailRecord::setFirstName(const QString &firstName)
{
    if (m_firstName == firstName)
        return;

    m_firstName = firstName;
    emit firstNameChanged();
}

QString QMLDebtorDetailRecord::lastName() const
{
    return m_lastName;
}

void QMLDebtorDetailRecord::setLastName(const QString &lastName)
{
    if (m_lastName == lastName)
        return;

    m_lastName = lastName;
    emit lastNameChanged();
}

QString QMLDebtorDetailRecord::phoneNumber() const
{
    return m_phoneNumber;
}

void QMLDebtorDetailRecord::setPhoneNumber(const QString &phoneNumber)
{
    if (m_phoneNumber == phoneNumber)
        return;

    m_phoneNumber = phoneNumber;
    emit phoneNumberChanged();
}

QStringList QMLDebtorDetailRecord::addressModel() const
{
    return m_addressModel;
}

void QMLDebtorDetailRecord::setAddressModel(const QStringList &addressModel)
{
    if (m_addressModel == addressModel)
        return;

    m_addressModel = addressModel;
    emit addressModelChanged();
}

QStringList QMLDebtorDetailRecord::emailModel() const
{
    return m_emailModel;
}

void QMLDebtorDetailRecord::setEmailModel(const QStringList &emailModel)
{
    if (m_emailModel == emailModel)
        return;

    m_emailModel = emailModel;
    emit emailModelChanged();
}

QDateTime QMLDebtorDetailRecord::created() const
{
    return m_created;
}

void QMLDebtorDetailRecord::setCreated(const QDateTime &created)
{
    if (m_created == created)
        return;

    m_created = created;
    emit createdChanged();
}

QDateTime QMLDebtorDetailRecord::lastEdited() const
{
    return m_lastEdited;
}

void QMLDebtorDetailRecord::setLastEdited(const QDateTime &lastEdited)
{
    if (m_lastEdited == lastEdited)
        return;

    m_lastEdited = lastEdited;
    emit lastEditedChanged();
}

int QMLDebtorDetailRecord::userId() const
{
    return m_userId;
}

void QMLDebtorDetailRecord::setUserId(int userId)
{
    if (m_userId == userId)
        return;

    m_userId = userId;
    emit userIdChanged();
}

QString QMLDebtorDetailRecord::user() const
{
    return m_user;
}

void QMLDebtorDetailRecord::setUser(const QString &user)
{
    if (m_user == user)
        return;

    m_user = user;
    emit userChanged();
}

void QMLDebtorDetailRecord::tryQuery()
{
    if (m_debtorId <= 0)
        return;

    setBusy(true);
    emit execute(new DebtorQuery::ViewDebtorDetails(m_debtorId,
                                                this));
}

void QMLDebtorDetailRecord::processResult(const QueryResult result)
{
    if (result.request().receiver() != this)
        return;

    setBusy(false);

    if (result.isSuccessful()) {
        if (result.request().command() == DebtorQuery::ViewDebtorDetails::COMMAND) {
            const QVariantMap &record = result.outcome().toMap().value("debtor").toMap();
            setPreferredName(record.value("preferred_name").toString());
            setFirstName(record.value("first_name").toString());
            setLastName(record.value("last_name").toString());
            setPhoneNumber(record.value("phone_number").toString());
            setAddressModel(record.value("addresses").toStringList());
            setEmailModel(record.value("emails").toStringList());
            setCreated(record.value("created").toDateTime());
            setLastEdited(record.value("last_edited").toDateTime());
            setUserId(record.value("user_id").toInt());
            setUser(record.value("user").toString());

            emit success(UnknownSuccess);
        }
    } else {
        emit error(UnknownError);
    }
}
