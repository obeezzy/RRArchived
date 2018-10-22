#include "qmluserprofile.h"
#include <QDebug>
#include <QSettings>

#include "database/databasethread.h"
#include "database/databaseexception.h"
#include "singletons/userprofile.h"

QMLUserProfile::QMLUserProfile(QObject *parent) :
    QObject(parent),
    m_busy(false)
{
    connect(this, &QMLUserProfile::executeRequest, &DatabaseThread::instance(), &DatabaseThread::execute);
    connect(&DatabaseThread::instance(), &DatabaseThread::resultReady, this, &QMLUserProfile::processResult);
}

QMLUserProfile::QMLUserProfile(DatabaseThread &thread) :
    m_busy(false)
{
    connect(this, &QMLUserProfile::executeRequest, &thread, &DatabaseThread::execute);
    connect(&thread, &DatabaseThread::resultReady, this, &QMLUserProfile::processResult);
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

void QMLUserProfile::signIn(const QString &userName, const QString &password)
{
    qInfo() << Q_FUNC_INFO << userName << password;
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
    qInfo() << Q_FUNC_INFO << userName << password;
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

void QMLUserProfile::removeUser(const QString &userName)
{
    qInfo() << Q_FUNC_INFO << userName;
    if (userName.trimmed().isEmpty()) {
        emit error(NoUserNameProvided);
    } else {
        setBusy(true);

        QueryRequest request(this);
        request.setCommand("remove_user", { { "user_name", userName } }, QueryRequest::User);
        emit executeRequest(request);
    }
}

void QMLUserProfile::processResult(const QueryResult &result)
{
    if (this != result.request().receiver())
        return;

    setBusy(false);

    if (result.isSuccessful()) {
        if (!result.outcome().toMap().isEmpty())
            UserProfile::instance().setUser(result.outcome().toMap().value("id").toInt(),
                                            result.outcome().toMap().value("user_name").toString());

        emit success();
    } else {
        switch (result.errorCode()) {
        case int(DatabaseException::RRErrorCode::SignInFailure):
            emit error(IncorrectCredentials);
            break;
        default:
            emit error(Unknown);
            break;
        }
    }
}
