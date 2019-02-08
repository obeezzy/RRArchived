#ifndef QMLUSERPROFILE_H
#define QMLUSERPROFILE_H

#include <QObject>
#include <QLoggingCategory>

class QueryRequest;
class QueryResult;
class DatabaseThread;

class QMLUserProfile : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool busy READ isBusy WRITE setBusy NOTIFY busyChanged)
    Q_PROPERTY(bool isFirstTime READ isFirstTime CONSTANT)
    Q_PROPERTY(bool isAdmin READ isAdmin NOTIFY adminChanged)
    Q_PROPERTY(QString userName READ userName NOTIFY userNameChanged)
public:
    explicit QMLUserProfile(QObject *parent = nullptr);
    explicit QMLUserProfile(DatabaseThread &);

    enum SuccessCode {
        UnknownSuccess,
        ChangePasswordSuccess,
        SignOutSuccess
    }; Q_ENUM(SuccessCode)

    enum ErrorCode {
        UnknownError,
        NoUserNameProvidedError,
        NoPasswordProvidedError,
        InvalidUserNameError,
        IncorrectCredentialsError,
        OldPasswordWrongError,
        UserAccountIsLockedError
    }; Q_ENUM(ErrorCode)

    bool isBusy() const;
    void setBusy(bool busy);

    bool isFirstTime() const;

    bool isAdmin() const;

    QString userName() const;

    Q_INVOKABLE void signIn(const QString &userName, const QString &password);
    Q_INVOKABLE void signInOnline(const QString &emailAddress, const QString &password);
    Q_INVOKABLE void signUp(const QString &userName, const QString &password);
    Q_INVOKABLE void signOut();
    Q_INVOKABLE void changePassword(const QString &oldPassword, const QString &newPassword);

    Q_INVOKABLE bool hasPrivilege(const QString &privilege);
signals:
    void executeRequest(const QueryRequest &);
    void busyChanged();
    void success(int successCode);
    void error(int errorCode);

    void isFirstTimeChanged();
    void adminChanged();
    void userNameChanged();
private slots:
    void processResult(const QueryResult &);
private:
    bool m_busy;
    bool m_admin;

    void setAdmin(bool admin);
};

Q_DECLARE_LOGGING_CATEGORY(qmlUserProfile);

#endif // QMLUSERPROFILE_H
