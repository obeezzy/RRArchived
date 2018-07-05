#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "qmlapi/qmluserprofile.h"
#include "databaseclient.h"

#include <QSqlQuery>

const QString USER_NAME("marines");
const QString PASSWORD("marines");

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
    void testRemoveUser();
    void testIncorrectCredentialsError();

private:
    QMLUserProfile *m_userProfile;
    DatabaseClient *m_client;
};

QMLUserProfileTest::QMLUserProfileTest()
{
    QLoggingCategory::setFilterRules(QStringLiteral("*.info=false"));
}

void QMLUserProfileTest::init()
{
    m_userProfile = new QMLUserProfile(this);
    m_client = new DatabaseClient;
}

void QMLUserProfileTest::cleanup()
{
    m_userProfile->deleteLater();
    delete m_client;
}

void QMLUserProfileTest::testSignUp()
{
    QSignalSpy successSpy(m_userProfile, &QMLUserProfile::success);
    QSignalSpy errorSpy(m_userProfile, &QMLUserProfile::error);

    QVERIFY(m_client->initialize());

    m_userProfile->signUp(USER_NAME, PASSWORD);
    QVERIFY(QTest::qWaitFor([&]() { return !m_userProfile->isBusy(); }, 2000));
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(errorSpy.count(), 0);

    QVERIFY(m_client->initialize());

    QSqlQuery q(m_client->connection());
    q.prepare("SELECT user FROM mysql.user WHERE user = :user_name");
    q.bindValue(":user_name", USER_NAME);
    QVERIFY(q.exec());
    QVERIFY(q.first());

    q.prepare("SELECT user FROM rr_test.user WHERE user = :user_name");
    q.bindValue(":user_name", USER_NAME);
    QVERIFY(q.exec());
    QVERIFY(q.first());
}

void QMLUserProfileTest::testSignIn()
{
    QSignalSpy successSpy(m_userProfile, &QMLUserProfile::success);
    QSignalSpy errorSpy(m_userProfile, &QMLUserProfile::error);

    QVERIFY(m_client->initialize());

    m_userProfile->signUp(USER_NAME, PASSWORD);
    QVERIFY(QTest::qWaitFor([&]() { return !m_userProfile->isBusy(); }, 2000));
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(errorSpy.count(), 0);
    successSpy.clear();
    errorSpy.clear();

    m_userProfile->signIn(USER_NAME, PASSWORD);
    QVERIFY(QTest::qWaitFor([&]() { return !m_userProfile->isBusy(); }, 2000));

    QVERIFY(m_client->initialize());
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(errorSpy.count(), 0);
}

void QMLUserProfileTest::testRemoveUser()
{
    QSignalSpy successSpy(m_userProfile, &QMLUserProfile::success);
    QSignalSpy errorSpy(m_userProfile, &QMLUserProfile::error);

    QVERIFY(m_client->initialize());

    m_userProfile->signUp(USER_NAME, PASSWORD);
    QVERIFY(QTest::qWaitFor([&]() { return !m_userProfile->isBusy(); }, 2000));
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(errorSpy.count(), 0);
    successSpy.clear();
    errorSpy.clear();

    m_userProfile->removeUser(USER_NAME);
    QVERIFY(QTest::qWaitFor([&]() { return !m_userProfile->isBusy(); }, 2000));
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(errorSpy.count(), 0);
}

void QMLUserProfileTest::testIncorrectCredentialsError()
{
    QVERIFY2(true, "Failure");
}

QTEST_MAIN(QMLUserProfileTest)

#include "tst_qmluserprofiletest.moc"
