#include "qmluserprofile.h"
#include <QDebug>
#include <QSettings>

#include "database/databasethread.h"
#include "database/databaseerror.h"
#include "network/networkurl.h"
#include "network/networkthread.h"
#include "network/serverrequest.h"
#include "network/serverresponse.h"
#include "network/networkerror.h"
#include "schema/schema.h"
#include "user/userprofile.h"
#include "user/businessdetails.h"
#include "user/businessadmin.h"
#include "user/businessstore.h"
#include "user/businessstoremodel.h"
#include "queryexecutors/user.h"

Q_LOGGING_CATEGORY(qmlUserProfile, "rrcore.qmlapi.qmluserprofile");

QMLUserProfile::QMLUserProfile(QObject *parent) :
    QMLUserProfile(DatabaseThread::instance(), parent)
{}

QMLUserProfile::QMLUserProfile(DatabaseThread &thread, QObject *parent) :
    QObject(parent),
    m_busy(false),
    m_businessStoreModel(new BusinessStoreModel(this))
{
    UserProfile::instance().setDatabaseReady(true);
    UserProfile::instance().setServerTunnelingEnabled(false);
    connect(this, &QMLUserProfile::execute, &thread, &DatabaseThread::execute);
    connect(&thread, &DatabaseThread::resultReady, this, &QMLUserProfile::processResult);

    connect(this, &QMLUserProfile::executeRequest,
            &NetworkThread::instance(), QOverload<ServerRequest>::of(&NetworkThread::execute));
    connect(&NetworkThread::instance(), &NetworkThread::responseReady, this, &QMLUserProfile::processServerResponse);

    connect(m_businessStoreModel, &BusinessStoreModel::currentIndexChanged,
            this, &QMLUserProfile::populateBusinessDetails);
}

bool QMLUserProfile::isBusy() const
{
    return m_busy;
}

void QMLUserProfile::setBusy(bool busy)
{
    if (m_busy == busy)
        return;

    m_busy = busy;
    emit busyChanged();
}

bool QMLUserProfile::isFirstTime() const
{
    return !UserProfile::instance().isDatabaseReady();
}

bool QMLUserProfile::isAdmin() const
{
    return UserProfile::instance().isAdmin();
}

QUrl QMLUserProfile::newUserUrl() const
{
    return NetworkUrl::NEW_USER_URL;
}

QUrl QMLUserProfile::passwordRetrievalUrl() const
{
    return NetworkUrl::PASSWORD_RETRIEVAL_URL;
}

QString QMLUserProfile::userName() const
{
    return UserProfile::instance().userName();
}

BusinessDetails *QMLUserProfile::businessDetails() const
{
    return UserProfile::instance().businessDetails();
}

BusinessStoreModel *QMLUserProfile::businessStoreModel() const
{
    return m_businessStoreModel;
}

void QMLUserProfile::signIn(const QString &userName, const QString &password)
{
    qCInfo(qmlUserProfile) << "signIn" << userName << password;
    if (userName.trimmed().isEmpty()) {
        emit error(NoUserNameProvided);
    } else if (password.isEmpty()) {
        emit error(NoPasswordProvided);
    } else {
        setBusy(true);
        emit execute(new UserQuery::SignInUser(userName, password, this));
    }
}

void QMLUserProfile::signInOnline(const QString &emailAddress, const QString &password)
{
    Q_UNUSED(emailAddress)
    Q_UNUSED(password)
    setBusy(true);
}

void QMLUserProfile::signUp(const QString &userName, const QString &password)
{
    qCInfo(qmlUserProfile) << "signUp" << userName << password;
    if (userName.trimmed().isEmpty()) {
        emit error(NoUserNameProvided);
    } else if (password.isEmpty()) {
        emit error(NoPasswordProvided);
    } else {
        setBusy(true);
        emit execute(new UserQuery::SignUpUser(userName, password, this));
    }
}

void QMLUserProfile::signOut()
{
    qCInfo(qmlUserProfile) << "signOut";
    setBusy(true);
    emit execute(new UserQuery::SignOutUser(this));
}

void QMLUserProfile::linkAccount(const QString &emailAddress, const QString &password)
{
    qCInfo(qmlUserProfile) << "linkAccount" << emailAddress << password;
    if (emailAddress.trimmed().isEmpty()) {
        emit error(NoEmailAddressProvided);
    } else if (password.isEmpty()) {
        emit error(NoPasswordProvided);
    } else {
        setBusy(true);

        ServerRequest request(this);
        request.setAction("link_account", { { "email_address", emailAddress }, { "password", password } });
        emit executeRequest(request);
    }
}

void QMLUserProfile::linkBusinessStore()
{
    setBusy(true);

    const BusinessStore &businessStore(UserProfile::instance().businessAdmin()->businessStores().at(m_businessStoreModel->currentIndex()));
    ServerRequest request(this);
    request.setAction("link_business_store", {
                          { "business_store", businessStore.toVariantMap() },
                          { "email_address", UserProfile::instance().businessAdmin()->emailAddress() },
                          { "password", UserProfile::instance().businessAdmin()->password() }
                      });
    emit executeRequest(request);
}

void QMLUserProfile::changePassword(const QString &oldPassword, const QString &newPassword)
{
    if (oldPassword.isEmpty() || newPassword.isEmpty()) {
        emit error(IncorrectCredentials);
        return;
    }

    emit execute(new UserQuery::ChangePassword(oldPassword, newPassword, this));
}

bool QMLUserProfile::hasPrivilege(const QString &privilege)
{
    if (privilege.trimmed().isEmpty())
        return false;

    return UserProfile::instance().hasPrivilege(privilege);
}

void QMLUserProfile::processResult(const QueryResult &result)
{
    if (this != result.request().receiver())
        return;

    setBusy(false);

    if (result.isSuccessful()) {
        if (result.request().command() == UserQuery::SignInUser::COMMAND) {
            UserProfile::instance().setUser(result.outcome().toMap().value("user_id").toInt(),
                                            result.outcome().toMap().value("user_name").toString(),
                                            result.outcome().toMap().value("password").toString(),
                                            result.outcome().toMap().value("user_privileges"),
                                            result.outcome().toMap().value("access_token").toByteArray());
            emit success(SignInSuccess);
        } else if (result.request().command() == UserQuery::SignUpUser::COMMAND) {
            UserProfile::instance().setUser(result.outcome().toMap().value("user_id").toInt(),
                                            result.outcome().toMap().value("user_name").toString(),
                                            result.outcome().toMap().value("password").toString(),
                                            result.outcome().toMap().value("user_privileges"),
                                            result.outcome().toMap().value("access_token").toByteArray());
            emit success(SignUpSuccess);
        } else if (result.request().command() == UserQuery::SignOutUser::COMMAND) {
            UserProfile::instance().clearUser();
            emit success(SignOutSuccess);
        } else if (result.request().command() == UserQuery::ChangePassword::COMMAND) {
            UserProfile::instance().setDatabaseReady(true);
            emit success(ChangePasswordSuccess);
        } else {
            emit success(UnknownSuccess);
        }
    } else {
        switch (result.errorCode()) {
        case DatabaseError::asInteger(DatabaseError::QueryErrorCode::UserAccountIsLocked):
            emit error(UserAccountIsLockedError);
            break;
        case DatabaseError::asInteger(DatabaseError::QueryErrorCode::SignInFailure):
            emit error(IncorrectCredentials);
            break;
        default:
            emit error(UnknownError);
            break;
        }
    }
}

void QMLUserProfile::processServerResponse(const ServerResponse &response)
{
    if (this != response.request().receiver())
        return;

    setBusy(false);
    if (response.isSuccessful()) {
        if (response.request().action() == "link_account") {
            UserProfile::instance()
                    .businessAdmin()->extractFromVariantMap(response.data().value("business_admin").toMap());
            emit success(LinkAccountSuccess);
        } else if (response.request().action() == "link_business_store") {
            const BusinessStore &businessStore(BusinessStore::fromVariantMap(response.data().value("business_store").toMap()));

            UserProfile::instance().setUserId(1); // User is definitely "admin"
            UserProfile::instance().setRackId(businessStore.rackId());
            UserProfile::instance().businessDetails()->extractFromBusinessStore(businessStore);
            UserProfile::instance().businessAdmin()->setPassword(QString()); // Clear password from memory
            emit success(LinkBusinessStoreSuccess);
        }
    } else {
        if (!response.hasError()) {
            switch (response.statusCode()) {
            case NetworkError::asInteger(NetworkError::StatusCode::ConnectionRefusedError):
                emit error(ConnectionRefusedError);
                break;
            default:
                emit error(UnknownError);
                break;
            }
        } else {
            switch (response.errorCode()) {
            case NetworkError::asInteger(NetworkError::ServerErrorCode::InvalidCredentialsError):
                emit error(IncorrectCredentials);
                break;
            default:
                emit error(UnknownError);
                break;
            }
        }
    }
}

void QMLUserProfile::populateBusinessDetails()
{
    const BusinessStore &businessStore(m_businessStoreModel->currentBusinessStore());
    UserProfile::instance().businessDetails()->extractFromBusinessStore(businessStore);
}
