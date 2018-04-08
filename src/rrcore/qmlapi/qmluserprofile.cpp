#include "qmluserprofile.h"
#include <QDebug>

#include "database/databasethread.h"
#include "database/databaseexception.h"
#include "singletons/userprofile.h"

QMLUserProfile::QMLUserProfile(QObject *parent) :
    QObject(parent),
    m_busy(false)
{
    connect(this, &QMLUserProfile::requestSent, &DatabaseThread::instance(), &DatabaseThread::execute);
    connect(&DatabaseThread::instance(), &DatabaseThread::resultsReady, this, &QMLUserProfile::processResult);
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

void QMLUserProfile::signIn(const QString &userName, const QString &password)
{
    qDebug() << Q_FUNC_INFO << userName << password;
    if (userName.trimmed().isEmpty()) {
        emit error(NoUserNameProvided);
    } else {
        setBusy(true);

        QueryRequest request(this);
        request.setCommand("sign_in_user", { { "user_name", userName }, { "password", password } }, QueryRequest::User);
        emit requestSent(request);
    }
}

void QMLUserProfile::signUp(const QString &userName, const QString &password)
{
    qDebug() << Q_FUNC_INFO << userName << password;
    setBusy(true);
}

void QMLUserProfile::processResult(const QueryResult &result)
{
    if (this != result.request().parent())
        return;

    setBusy(false);

    if (result.isSuccessful()) {
        if (!result.outcome().toMap().isEmpty())
            UserProfile::instance().setUser(result.outcome().toMap().value("id").toInt(),
                                            result.outcome().toMap().value("user_name").toString());

        emit success();
    } else {
        switch (result.errorCode()) {
        case DatabaseException::SignInFailure:
            emit error(IncorrectCredentials);
            break;
        default:
            emit error(Unknown);
            break;
        }
    }
}
