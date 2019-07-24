#ifndef QMLUSERDETAILRECORD_H
#define QMLUSERDETAILRECORD_H

#include "detailrecord/abstractdetailrecord.h"
#include <QUrl>

class QMLUserDetailRecord : public AbstractDetailRecord
{
    Q_OBJECT
    Q_PROPERTY(int userId READ userId WRITE setUserId NOTIFY userIdChanged)
    Q_PROPERTY(QString firstName READ firstName NOTIFY firstNameChanged)
    Q_PROPERTY(QString lastName READ lastName NOTIFY lastNameChanged)
    Q_PROPERTY(QString userName READ userName NOTIFY userNameChanged)
    Q_PROPERTY(QString phoneNumber READ phoneNumber NOTIFY phoneNumberChanged)
    Q_PROPERTY(QString emailAddress READ emailAddress NOTIFY emailAddressChanged)
    Q_PROPERTY(QUrl imageUrl READ imageUrl NOTIFY imageUrlChanged)
public:
    explicit QMLUserDetailRecord(QObject *parent = nullptr);
    explicit QMLUserDetailRecord(DatabaseThread &thread, QObject *parent = nullptr);

    int userId() const;
    void setUserId(int userId);

    QString firstName() const;
    QString lastName() const;
    QString userName() const;

    QString phoneNumber() const;
    QString emailAddress() const;
    QUrl imageUrl() const;
signals:
    void userIdChanged();
    void firstNameChanged();
    void lastNameChanged();
    void userNameChanged();
    void phoneNumberChanged();
    void emailAddressChanged();
    void imageUrlChanged();
protected:
    void tryQuery() override;
    void processResult(const QueryResult result) override;
private:
    int m_userId;
    QString m_firstName;
    QString m_lastName;
    QString m_userName;
    QString m_phoneNumber;
    QString m_emailAddress;
    QUrl m_imageUrl;

    void setFirstName(const QString &firstName);
    void setLastName(const QString &lastName);
    void setUserName(const QString &userName);
    void setPhoneNumber(const QString &phoneNumber);
    void setEmailAddress(const QString &emailAddress);
    void setImageUrl(const QUrl &imageUrl);
};

#endif // QMLUSERDETAILRECORD_H
