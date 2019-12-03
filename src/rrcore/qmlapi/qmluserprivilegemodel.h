#ifndef QMLUSERPRIVILEGEMODEL_H
#define QMLUSERPRIVILEGEMODEL_H

#include "models/abstractvisuallistmodel.h"
#include <QUrl>

class UserPrivilegeModel;

class QMLUserPrivilegeModel : public AbstractVisualListModel
{
    Q_OBJECT
    Q_PROPERTY(int userId READ userId WRITE setUserId NOTIFY userIdChanged)
    Q_PROPERTY(QString firstName READ firstName WRITE setFirstName NOTIFY firstNameChanged)
    Q_PROPERTY(QString lastName READ lastName WRITE setLastName NOTIFY lastNameChanged)
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(QUrl imageUrl READ imageUrl WRITE setImageUrl NOTIFY imageUrlChanged)
    Q_PROPERTY(QString phoneNumber READ phoneNumber WRITE setPhoneNumber NOTIFY phoneNumberChanged)
    Q_PROPERTY(QString emailAddress READ emailAddress WRITE setEmailAddress NOTIFY emailAddressChanged)
public:
    enum Roles {
        PrivilegeGroupRole = Qt::UserRole,
        TitleRole,
        PrivilegeModelRole
    };

    enum SuccessCode {
        AddUserSuccess,
        UpdateUserSuccess
    }; Q_ENUM (SuccessCode)

    enum ErrorCode {
        NoFirstNameSetError,
        NoLastNameSetError,
        NoUserNameSetError,
        NoPasswordSetError,
        NoPhoneNumberSetError,
        NoEmailAddressSetError,
        UserAlreadyExistsError,
        ImageTooLargeError,
        UserPreviouslyArchivedError
    }; Q_ENUM(ErrorCode)

    explicit QMLUserPrivilegeModel(QObject *parent = nullptr);
    explicit QMLUserPrivilegeModel(DatabaseThread &thread, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    bool isExistingUser() const;

    int userId() const;
    void setUserId(int userId);

    QString firstName() const;
    void setFirstName(const QString &firstName);

    QString lastName() const;
    void setLastName(const QString &lastName);

    QString userName() const;
    void setUserName(const QString &userName);

    QString password() const;
    void setPassword(const QString &password);

    QUrl imageUrl() const;
    void setImageUrl(const QUrl &imageUrl);

    QString phoneNumber() const;
    void setPhoneNumber(const QString &phoneNumber);

    QString emailAddress() const;
    void setEmailAddress(const QString &emailAddress);

    Q_INVOKABLE void setPrivilegeValue(int groupIndex, int privilegeIndex, bool value);
signals:
    void userIdChanged();
    void firstNameChanged();
    void lastNameChanged();
    void userNameChanged();
    void passwordChanged();
    void imageUrlChanged();
    void phoneNumberChanged();
    void emailAddressChanged();
public slots:
    bool submit() override;
protected:
    void tryQuery() override;
    void processResult(const QueryResult result) override;
private:
    QStringList m_titles;
    QStringList m_privilegeGroups;
    QList<UserPrivilegeModel *> m_privilegeModels;
    int m_userId;
    QString m_firstName;
    QString m_lastName;
    QString m_userName;
    QString m_password;
    QUrl m_imageUrl;
    QString m_phoneNumber;
    QString m_emailAddress;
};

#endif // QMLUSERPRIVILEGEMODEL_H
