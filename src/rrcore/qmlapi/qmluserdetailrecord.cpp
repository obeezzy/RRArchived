#include "qmluserdetailrecord.h"
#include "database/databasethread.h"
#include "queryexecutors/user.h"
#include "utility/userutils.h"

using namespace Utility;

QMLUserDetailRecord::QMLUserDetailRecord(QObject *parent) :
    QMLUserDetailRecord(DatabaseThread::instance(), parent)
{}

QMLUserDetailRecord::QMLUserDetailRecord(DatabaseThread &thread,
                                         QObject *parent) :
    AbstractDetailRecord (thread, parent)
{}

int QMLUserDetailRecord::userId() const
{
    return m_user.id;
}

void QMLUserDetailRecord::setUserId(int userId)
{
    if (m_user.id == userId)
        return;

    m_user.id = userId;
    emit userIdChanged();
}

QString QMLUserDetailRecord::firstName() const
{
    return m_user.firstName;
}

void QMLUserDetailRecord::setFirstName(const QString &firstName)
{
    if (m_user.firstName == firstName)
        return;

    m_user.firstName = firstName;
    emit firstNameChanged();
}

QString QMLUserDetailRecord::lastName() const
{
    return m_user.lastName;
}

void QMLUserDetailRecord::setLastName(const QString &lastName)
{
    if (m_user.lastName == lastName)
        return;

    m_user.lastName = lastName;
    emit lastNameChanged();
}

QString QMLUserDetailRecord::userName() const
{
    return m_user.user;
}

void QMLUserDetailRecord::setUserName(const QString &userName)
{
    if (m_user.user == userName)
        return;

    m_user.user = userName;
    emit userNameChanged();
}

QString QMLUserDetailRecord::phoneNumber() const
{
    return m_user.phoneNumber;
}

void QMLUserDetailRecord::setPhoneNumber(const QString &phoneNumber)
{
    if (m_user.phoneNumber == phoneNumber)
        return;

    m_user.phoneNumber = phoneNumber;
    emit phoneNumberChanged();
}

QString QMLUserDetailRecord::emailAddress() const
{
    return m_user.emailAddress;
}

QUrl QMLUserDetailRecord::imageUrl() const
{
    return m_user.imageUrl;
}

void QMLUserDetailRecord::setImageUrl(const QUrl &imageUrl)
{
    if (m_user.imageUrl == imageUrl)
        return;

    m_user.imageUrl = imageUrl;
    emit imageUrlChanged();
}

void QMLUserDetailRecord::setEmailAddress(const QString &emailAddress)
{
    if (m_user.emailAddress == emailAddress)
        return;

    m_user.emailAddress = emailAddress;
    emit emailAddressChanged();
}

void QMLUserDetailRecord::tryQuery()
{
    setBusy(true);
    emit execute(new UserQuery::FetchUser(m_user.id, this));
}

void QMLUserDetailRecord::processResult(const QueryResult result)
{
    setBusy(false);
    if (result.isSuccessful()) {
        const auto user = Utility::User{ result.outcome().toMap() };
        setFirstName(user.firstName);
        setLastName(user.lastName);
        setUserName(user.user);
        setPhoneNumber(user.phoneNumber);
        setEmailAddress(user.emailAddress);
        setImageUrl(user.imageUrl);

        emit success();
    } else {
        emit error();
    }
}
