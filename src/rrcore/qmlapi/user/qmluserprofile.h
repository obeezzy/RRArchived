#ifndef QMLUSERPROFILE_H
#define QMLUSERPROFILE_H

#include <QLoggingCategory>
#include <QObject>
#include <QUrl>
#include "database/queryexecutor.h"
#include "models/modelresult.h"
#include "user/businessstore.h"
#include "utility/user/user.h"

class QueryRequest;
class QueryResult;
class DatabaseThread;
class ServerRequest;
class ServerResponse;
class BusinessDetails;
class BusinessStoreModel;

class QMLUserProfile : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool busy READ isBusy WRITE setBusy NOTIFY busyChanged)
    Q_PROPERTY(bool isFirstTime READ isFirstTime CONSTANT)
    Q_PROPERTY(bool isAdmin READ isAdmin NOTIFY adminChanged)
    Q_PROPERTY(QString userName READ userName NOTIFY userNameChanged)
    Q_PROPERTY(QUrl newUserUrl READ newUserUrl CONSTANT)
    Q_PROPERTY(QUrl passwordRetrievalUrl READ passwordRetrievalUrl CONSTANT)
    Q_PROPERTY(BusinessDetails* businessDetails READ businessDetails CONSTANT)
    Q_PROPERTY(BusinessStoreModel* businessStoreModel READ businessStoreModel
                   NOTIFY businessStoreModelChanged)
public:
    explicit QMLUserProfile(QObject* parent = nullptr);
    explicit QMLUserProfile(DatabaseThread&, QObject* parent = nullptr);

    enum SuccessCode
    {
        SignInSuccess,
        SignUpSuccess,
        LinkAccountSuccess,
        LinkBusinessStoreSuccess,
        ChangePasswordSuccess,
        SignOutSuccess
    };
    Q_ENUM(SuccessCode)

    enum ErrorCode
    {
        NoUserNameProvided,
        NoEmailAddressProvided,
        NoPasswordProvided,
        InvalidUserName,
        IncorrectCredentials,
        OldPasswordWrongError,
        UserAccountIsLockedError,
        ConnectionRefusedError
    };
    Q_ENUM(ErrorCode)

    bool isBusy() const;
    void setBusy(bool busy);

    bool isFirstTime() const;
    bool isAdmin() const;

    QUrl newUserUrl() const;
    QUrl passwordRetrievalUrl() const;

    QString userName() const;

    BusinessDetails* businessDetails() const;
    BusinessStoreModel* businessStoreModel() const;

    Q_INVOKABLE void signIn(const QString& userName, const QString& password);
    Q_INVOKABLE void signInOnline(const QString& emailAddress,
                                  const QString& password);
    Q_INVOKABLE void signUp(const QString& userName, const QString& password);
    Q_INVOKABLE void signOut();
    Q_INVOKABLE void linkAccount(const QString& emailAddress,
                                 const QString& password);
    Q_INVOKABLE void linkBusinessStore();
    Q_INVOKABLE void changePassword(const QString& oldPassword,
                                    const QString& newPassword);
    Q_INVOKABLE bool hasPrivilege(const QString& privilege);
signals:
    void execute(QueryExecutor*);
    void executeRequest(const ServerRequest&);
    void busyChanged();
    void success(ModelResult result = ModelResult());
    void error(ModelResult result = ModelResult());
    void businessStoreModelChanged();

    void isFirstTimeChanged();
    void adminChanged();
    void userNameChanged();
private slots:
    void processResult(const QueryResult&);
    void processServerResponse(const ServerResponse&);

private:
    bool m_busy{false};
    bool m_admin{false};
    BusinessStoreModel* m_businessStoreModel;
    Utility::User::User m_user;

    void setAdmin(bool admin);

    void populateBusinessDetails();
};

Q_DECLARE_LOGGING_CATEGORY(lcqmluserprofile);

#endif  // QMLUSERPROFILE_H
