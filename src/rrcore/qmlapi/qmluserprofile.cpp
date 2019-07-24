#include "qmluserprofile.h"
#include <QDebug>
#include <QSettings>

#include "database/databasethread.h"
#include "database/databaseexception.h"
#include "singletons/userprofile.h"

Q_LOGGING_CATEGORY(qmlUserProfile, "rrcore.qmlapi.qmluserprofile");

QMLUserProfile::QMLUserProfile(QObject *parent) :
    QMLUserProfile(DatabaseThread::instance(), parent)
{}

QMLUserProfile::QMLUserProfile(DatabaseThread &thread, QObject *parent) :
    QObject(parent),
    m_busy(false)
{
    connect(this, &QMLUserProfile::executeRequest, &thread, &DatabaseThread::execute);
    connect(&thread, &DatabaseThread::resultReady, this, &QMLUserProfile::processResult);

    //connect(UserProfile::instance(), &UserProfile::adminChanged, this, &QMLUserProfile::adminChanged);
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
    return QSettings().value("is_first_time", true).toBool();
}

bool QMLUserProfile::isAdmin() const
{
    return UserProfile::instance().isAdmin();
}

QString QMLUserProfile::userName() const
{
    return UserProfile::instance().userName();
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

        QueryRequest request(this);
        request.setCommand("sign_in_user", { { "user_name", userName }, { "password", password } }, QueryRequest::User);
        emit executeRequest(request);
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

        QueryRequest request(this);
        request.setCommand("sign_up_user", { { "user_name", userName }, { "password", password } }, QueryRequest::User);
        emit executeRequest(request);
    }
}

void QMLUserProfile::signOut()
{
    qCInfo(qmlUserProfile) << "signOut";
    QueryRequest request(this);
    request.setCommand("sign_out_user", { }, QueryRequest::User);
    emit executeRequest(request);
}

void QMLUserProfile::changePassword(const QString &oldPassword, const QString &newPassword)
{
    if (oldPassword.isEmpty() || newPassword.isEmpty()) {
        emit error(IncorrectCredentials);
        return;
    }

    QueryRequest request(this);
    request.setCommand("change_password", { { "old_password", oldPassword }, { "new_password", newPassword } }, QueryRequest::User);
    emit executeRequest(request);
}

bool QMLUserProfile::hasPrivilege(const QString &privilege)
{
    if (privilege.trimmed().isEmpty())
        return false;

    return UserProfile::instance().hasPrivilege(privilege);
}

void QMLUserProfile::processResult(const QueryResult &result)
{
    qDebug() << "Returned result!" << result;
    if (this != result.request().receiver())
        return;

    setBusy(false);

    if (result.isSuccessful()) {
        if (result.request().command() == "sign_in_user") {
            emit success(SignInSuccess);
        } else if (result.request().command() == "sign_up_user") {
            emit success(SignUpSuccess);
        } else if (result.request().command() == "sign_out_user") {
            emit success(SignOutSuccess);
        } else if (result.request().command() == "change_password") {
            QSettings().setValue("is_first_time", false);
            emit success(ChangePasswordSuccess);
        } else if (!result.outcome().toMap().isEmpty()) {
            UserProfile::instance().setUser(result.outcome().toMap().value("user_id").toInt(),
                                            result.outcome().toMap().value("user_name").toString(),
                                            result.outcome().toMap().value("user_privileges")
                                            );

            emit success(UnknownSuccess);
        }
    } else {
        switch (result.errorCode()) {
        case static_cast<int>(DatabaseException::RRErrorCode::UserAccountIsLocked):
            emit error(UserAccountIsLockedError);
            break;
        case static_cast<int>(DatabaseException::RRErrorCode::SignInFailure):
            emit error(IncorrectCredentials);
            break;
        default:
            emit error(UnknownError);
            break;
        }
    }
}
