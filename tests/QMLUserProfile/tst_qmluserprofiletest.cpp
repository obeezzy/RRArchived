#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "qmlapi/qmluserprofile.h"
#include "database/databaseexception.h"
#include "mockdatabasethread.h"

class QMLUserProfileTest : public QObject
{
    Q_OBJECT

public:
    QMLUserProfileTest();

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
    MockDatabaseThread m_thread;
    QueryResult m_result;
};

QMLUserProfileTest::QMLUserProfileTest() :
    m_thread(&m_result)
{
    QLoggingCategory::setFilterRules(QStringLiteral("*.info=false"));
}

void QMLUserProfileTest::init()
{
    m_userProfile = new QMLUserProfile(m_thread);
}

void QMLUserProfileTest::cleanup()
{
    m_userProfile->deleteLater();
}

void QMLUserProfileTest::testSignUp()
{
    auto databaseWillReturnEmptyResult = [this]() {
        m_result.setSuccessful(true);
        m_result.setOutcome(QVariant());
    };
    QSignalSpy successSpy(m_userProfile, &QMLUserProfile::success);
    QSignalSpy errorSpy(m_userProfile, &QMLUserProfile::error);

    databaseWillReturnEmptyResult();

    m_userProfile->signUp(QStringLiteral("marines"), QStringLiteral("marines"));
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(errorSpy.count(), 0);
}

void QMLUserProfileTest::testSignIn()
{
    auto databaseWillReturnEmptyResult = [this]() {
        m_result.setSuccessful(true);
        m_result.setOutcome(QVariant());
    };
    QSignalSpy successSpy(m_userProfile, &QMLUserProfile::success);
    QSignalSpy errorSpy(m_userProfile, &QMLUserProfile::error);

    databaseWillReturnEmptyResult();

    m_userProfile->signIn(QStringLiteral("marines"), QStringLiteral("marines"));

    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(errorSpy.count(), 0);
}

void QMLUserProfileTest::testIncorrectCredentialsError()
{
    auto databaseWillReturnSignInFailure = [this]() {
        m_result.setSuccessful(false);
        m_result.setOutcome(QVariant());
        m_result.setErrorCode(static_cast<int>(DatabaseException::RRErrorCode::SignInFailure));
    };
    QSignalSpy successSpy(m_userProfile, &QMLUserProfile::success);
    QSignalSpy errorSpy(m_userProfile, &QMLUserProfile::error);

    databaseWillReturnSignInFailure();

    m_userProfile->signIn(QStringLiteral("marines"), QStringLiteral("marines"));
    QCOMPARE(successSpy.count(), 0);
    QCOMPARE(errorSpy.count(), 1);
    QCOMPARE(errorSpy.takeFirst().first().value<QMLUserProfile::ErrorCode>(), QMLUserProfile::IncorrectCredentialsError);
}

void QMLUserProfileTest::testNoUserNameProvidedError()
{
    QSignalSpy successSpy(m_userProfile, &QMLUserProfile::success);
    QSignalSpy errorSpy(m_userProfile, &QMLUserProfile::error);

    m_userProfile->signIn(QStringLiteral(""), QStringLiteral("marines"));
    QCOMPARE(successSpy.count(), 0);
    QCOMPARE(errorSpy.count(), 1);
    QCOMPARE(errorSpy.takeFirst().first().value<QMLUserProfile::ErrorCode>(), QMLUserProfile::NoUserNameProvidedError);
}

void QMLUserProfileTest::testNoPasswordProvidedError()
{
    QSignalSpy successSpy(m_userProfile, &QMLUserProfile::success);
    QSignalSpy errorSpy(m_userProfile, &QMLUserProfile::error);

    m_userProfile->signIn(QStringLiteral("marines"), QStringLiteral(""));
    QCOMPARE(successSpy.count(), 0);
    QCOMPARE(errorSpy.count(), 1);
    QCOMPARE(errorSpy.takeFirst().first().value<QMLUserProfile::ErrorCode>(), QMLUserProfile::NoPasswordProvidedError);
}

QTEST_MAIN(QMLUserProfileTest)

#include "tst_qmluserprofiletest.moc"
