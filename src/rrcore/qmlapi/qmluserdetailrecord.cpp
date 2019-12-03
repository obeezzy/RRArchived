#include "qmluserdetailrecord.h"
#include "database/databasethread.h"
#include "queryexecutors/user.h"

QMLUserDetailRecord::QMLUserDetailRecord(QObject *parent) :
    QMLUserDetailRecord(DatabaseThread::instance(), parent)
{}

QMLUserDetailRecord::QMLUserDetailRecord(DatabaseThread &thread, QObject *parent) :
    AbstractDetailRecord (thread, parent),
    m_userId(-1)
{

}

int QMLUserDetailRecord::userId() const
{
    return m_userId;
}

void QMLUserDetailRecord::setUserId(int userId)
{
    if (m_userId == userId)
        return;

    m_userId = userId;
    emit userIdChanged();
}

QString QMLUserDetailRecord::firstName() const
{
    return m_firstName;
}

void QMLUserDetailRecord::setFirstName(const QString &firstName)
{
    if (m_firstName == firstName)
        return;

    m_firstName = firstName;
    emit firstNameChanged();
}

QString QMLUserDetailRecord::lastName() const
{
    return m_lastName;
}

void QMLUserDetailRecord::setLastName(const QString &lastName)
{
    if (m_lastName == lastName)
        return;

    m_lastName = lastName;
    emit lastNameChanged();
}

QString QMLUserDetailRecord::userName() const
{
    return m_userName;
}

void QMLUserDetailRecord::setUserName(const QString &userName)
{
    if (m_userName == userName)
        return;

    m_userName = userName;
    emit userNameChanged();
}

QString QMLUserDetailRecord::phoneNumber() const
{
    return m_phoneNumber;
}

void QMLUserDetailRecord::setPhoneNumber(const QString &phoneNumber)
{
    if (m_phoneNumber == phoneNumber)
        return;

    m_phoneNumber = phoneNumber;
    emit phoneNumberChanged();
}

QString QMLUserDetailRecord::emailAddress() const
{
    return m_emailAddress;
}

QUrl QMLUserDetailRecord::imageUrl() const
{
    return m_imageUrl;
}

void QMLUserDetailRecord::setImageUrl(const QUrl &imageUrl)
{
    if (m_imageUrl == imageUrl)
        return;

    m_imageUrl = imageUrl;
    emit imageUrlChanged();
}

void QMLUserDetailRecord::setEmailAddress(const QString &emailAddress)
{
    if (m_emailAddress == emailAddress)
        return;

    m_emailAddress = emailAddress;
    emit emailAddressChanged();
}

void QMLUserDetailRecord::tryQuery()
{
    setBusy(true);
    emit execute(new UserQuery::ViewUserDetails(m_userId, this));
}

void QMLUserDetailRecord::processResult(const QueryResult result)
{
    if (this != result.request().receiver())
        return;

    setBusy(false);
    if (result.isSuccessful()) {
        setFirstName(result.outcome().toMap().value("user").toMap().value("first_name").toString());
        setLastName(result.outcome().toMap().value("user").toMap().value("last_name").toString());
        setUserName(result.outcome().toMap().value("user").toMap().value("user_name").toString());
        setPhoneNumber(result.outcome().toMap().value("user").toMap().value("phone_number").toString());
        setEmailAddress(result.outcome().toMap().value("user").toMap().value("email_address").toString());
        setImageUrl(result.outcome().toMap().value("user").toMap().value("image_url").toUrl());

        emit success();
    } else {
        emit error();
    }
}
