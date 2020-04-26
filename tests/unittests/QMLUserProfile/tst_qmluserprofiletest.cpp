#include "qmlapi/qmluserprofile.h"
#include "database/databaseerror.h"
#include "mockdatabasethread.h"
#include <QString>
#include <QtTest>
#include <QCoreApplication>

class QMLUserProfileTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void init();
    void cleanup();

    void testSignUp();
    void testSignIn();
    void testIncorrectCredentialsError();
    void testNoUserNameProvidedError();
    void testNoPasswordProvidedError();
private:
    QMLUserProfile *m_userProfile;
    MockDatabaseThread *m_thread;
};

void QMLUserProfileTest::init()
{
    m_thread = new MockDatabaseThread(this);
    m_userProfile = new QMLUserProfile(*m_thread, this);
}

void QMLUserProfileTest::cleanup()
{
    m_userProfile->deleteLater();
    m_thread->deleteLater();
}

void QMLUserProfileTest::testSignUp()
{
    auto databaseWillReturnEmptyResult = [this]() {
        m_thread->result().setSuccessful(true);
    };
    QSignalSpy successSpy(m_userProfile, &QMLUserProfile::success);
    QSignalSpy errorSpy(m_userProfile, &QMLUserProfile::error);

    databaseWillReturnEmptyResult();

    m_userProfile->signUp(QStringLiteral("marines"), QStringLiteral("marines"));
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(successSpy.takeFirst().first().value<ModelResult>().code(),
             QMLUserProfile::SignUpSuccess);
}

void QMLUserProfileTest::testSignIn()
{
    auto databaseWillReturnEmptyResult = [this]() {
        m_thread->result().setSuccessful(true);
    };
    QSignalSpy successSpy(m_userProfile, &QMLUserProfile::success);
    QSignalSpy errorSpy(m_userProfile, &QMLUserProfile::error);

    databaseWillReturnEmptyResult();

    m_userProfile->signIn(QStringLiteral("marines"), QStringLiteral("marines"));

    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(successSpy.takeFirst().at(0).toInt(),
             static_cast<int>(QMLUserProfile::SignInSuccess));
}

void QMLUserProfileTest::testIncorrectCredentialsError()
{
    auto databaseWillReturnSignInFailure = [this]() {
        m_thread->result().setSuccessful(false);
        m_thread->result().setErrorCode(static_cast<int>(DatabaseError::QueryErrorCode::InvalidCredentialsError));
    };
    QSignalSpy successSpy(m_userProfile, &QMLUserProfile::success);
    QSignalSpy errorSpy(m_userProfile, &QMLUserProfile::error);

    databaseWillReturnSignInFailure();

    m_userProfile->signIn(QStringLiteral("marines"), QStringLiteral("marines"));
    QCOMPARE(successSpy.count(), 0);
    QCOMPARE(errorSpy.count(), 1);
    QCOMPARE(errorSpy.takeFirst().first().value<ModelResult>().code(),
             QMLUserProfile::IncorrectCredentials);
}

void QMLUserProfileTest::testNoUserNameProvidedError()
{
    QSignalSpy successSpy(m_userProfile, &QMLUserProfile::success);
    QSignalSpy errorSpy(m_userProfile, &QMLUserProfile::error);

    m_userProfile->signIn(QStringLiteral(""), QStringLiteral("marines"));
    QCOMPARE(successSpy.count(), 0);
    QCOMPARE(errorSpy.count(), 1);
    QCOMPARE(errorSpy.takeFirst().first().value<ModelResult>().code(),
             QMLUserProfile::NoUserNameProvided);
}

void QMLUserProfileTest::testNoPasswordProvidedError()
{
    QSignalSpy successSpy(m_userProfile, &QMLUserProfile::success);
    QSignalSpy errorSpy(m_userProfile, &QMLUserProfile::error);

    m_userProfile->signIn(QStringLiteral("marines"), QStringLiteral(""));
    QCOMPARE(successSpy.count(), 0);
    QCOMPARE(errorSpy.count(), 1);
    QCOMPARE(errorSpy.takeFirst().first().value<ModelResult>().code(),
             QMLUserProfile::NoPasswordProvided);
}

QTEST_GUILESS_MAIN(QMLUserProfileTest)

#include "tst_qmluserprofiletest.moc"
